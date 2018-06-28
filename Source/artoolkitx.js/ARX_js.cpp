/**
 *  
 * 
 */

#include <emscripten/bind.h>
#include "ARX/ARX_c.h"
#include "ARX_c.cpp"

#define PIXEL_FORMAT_BUFFER_SIZE 1024

using namespace emscripten;

std::string getARToolKitVersion(){
    char versionString[1024];
    std::string returnValue ("unknown version");
    if (arwGetARToolKitVersion(versionString, 1024)){
        return std::string(versionString);
    }
    return returnValue;
}

ARParamLT *paramLT = NULL;
ARHandle * arhandle = NULL;
AR3DHandle *ar3DHandle = NULL;
ARPattHandle *arPattHandle = NULL;

void deleteHandle() {
    if (arhandle != NULL) {
        arPattDetach(arhandle);
        arDeleteHandle(arhandle);
        arhandle = NULL;
    }
    if (ar3DHandle != NULL) {
        ar3DDeleteHandle(&(ar3DHandle));
        ar3DHandle = NULL;
    }
    if (paramLT != NULL) {
        arParamLTFree(&(paramLT));
        paramLT = NULL;
    }
}

int teardown() {

    deleteHandle();
    arPattDeleteHandle(arPattHandle);

    // for (int i=0; i<arc->multi_markers.size(); i++) {
    //     arMultiFreeConfig(arc->multi_markers[i].multiMarkerHandle);
    // }

    return 0;
}

int addTrackable(std::string cfg) {
    return arwAddTrackable(cfg.c_str());
}

ARController* returnARController() {
    return gARTK;
}

std::shared_ptr<ARTrackerSquare> getSquareTracker() {
    return gARTK->getSquareTracker();
}
int loadCamera(std::string cparam_name, ARParam *param) {
    if (arParamLoad(cparam_name.c_str(), 1, param) < 0) {
        ARLOGe("loadCamera(): Error loading parameter file %s for camera.\n", cparam_name.c_str());
        return -1;
    }
    // int cameraID = gCameraID++;
    // cameraParams[cameraID] = param;
    // return cameraID;

    int cameraId  = 0;
    return cameraId;
}

int setCamera(ARParam *param, int width, int height) {

    if (param->xsize != width || param->ysize != height) {
        ARLOGw("*** Camera Parameter resized from %d, %d. ***\n", param->xsize, param->ysize);
        arParamChangeSize(param, width, height, param);
    }

    deleteHandle();
    
    if ((paramLT = arParamLTCreate(param, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
        ARLOGe("setCamera(): Error: arParamLTCreate.\n");
        return -1;
    }

    // ARLOGi("setCamera(): arParamLTCreated\n..%d, %d\n", (arc->paramLT->param).xsize, (arc->paramLT->param).ysize);

    // setup camera
    
    if ((arhandle = arCreateHandle(paramLT)) == NULL) {
        ARLOGe("setCamera(): Error: arCreateHandle.\n");
        return -1;
    }
    arSetPixelFormat(arhandle, AR_PIXEL_FORMAT_RGBA);

    // ARLOGi("setCamera(): arCreateHandle done\n");


    ar3DHandle = ar3DCreateHandle(param);
    if (ar3DHandle == NULL) {
        ARLOGe("setCamera(): Error creating 3D handle");
        return -1;
    }

    // ARLOGi("setCamera(): ar3DCreateHandle done\n");

    arPattAttach(arhandle, arPattHandle);
    // ARLOGi("setCamera(): Pattern handler attached.\n");
    return 0;
}


extern "C" {
    int detectMarker(ARUint8 *videoFrame, ARUint8 *videoLuma) {
        AR2VideoBufferT buff = {0};
        buff.buff = videoFrame;
        buff.fillFlag = 1;

        buff.buffLuma = videoLuma;
        ARHandle* handle = arCreateHandle(NULL);
        return arDetectMarker(handle, &buff);
    }

	/**
	 * Initialises and starts video capture.
	 * @param vconf		The video configuration string
	 * @param cparaBuff	A string containing the contents of a camera parameter file, which is used to form the projection matrix.
	 * @param cparaBuffLen	Number of characters in cparaBuff.
	 * @return			true if successful, false if an error occurred
	 * @see				arwStopRunning()
	 */
    bool arwStartRunningJS(std::string cparaName, int width, int height) {
        ARParam param;
        
        if(loadCamera(cparaName, &param) < 0 ) {
            return false;
        }
        setCamera(&param, width, height);

        return true;
    }
}
//setup function needed
// arSetPixelFormat()


struct VideoParams {
    int width;
    int height;
    int pixelSize;
    std::string pixelFormat;
};
VideoParams getVideoParams() {

    int w, h, ps;
    char pf[PIXEL_FORMAT_BUFFER_SIZE];
    VideoParams videoParams;
    if( !arwGetVideoParams(&w, &h, &ps, pf, PIXEL_FORMAT_BUFFER_SIZE))
        return videoParams;
    else {
        videoParams.width = w;
        videoParams.height = h;
        videoParams.pixelSize = ps;
        videoParams.pixelFormat = std::string(pf);
    }
    return videoParams;
}

//TODO: to be implemented
// bool getTrackables() {
//     int count;
//     ARWTrackableStatus status;
//     arwGetTrackables(&count, status)
// }

EMSCRIPTEN_BINDINGS(constant_bindings) {

    function("setLogLevel", &arwSetLogLevel);

    /*** artoolkitX lifecycle functions ***/
    function("initialiseAR", &arwInitialiseAR);
    function("getARToolKitVersion", &getARToolKitVersion);
    function("arwStartRunningJS", &arwStartRunningJS);
    function("getError", &arwGetError);
    
    function("isRunning", &arwIsRunning);
    function("isInitialized", &arwIsInited);
    function("stopRunning", &arwStopRunning);
    function("shutdownAR", &arwShutdownAR);

    /*** Video stream management ***/
    //* ATTENTION: arwGetProjectionMatrix is exported from ARX_additions.js

    value_object<VideoParams>("VideoParams")
        .field("width", &VideoParams::width)
        .field("height", &VideoParams::height)
        .field("pixelSize", &VideoParams::pixelSize)
        .field("pixelFormat", &VideoParams::pixelFormat);
    function("getVideoParams", &getVideoParams);

    function("capture", &arwCapture);
    function("updateAR", &arwUpdateAR);

    /*** Video stream retrieval and/or drawing ***/



    /*** Tracking configuration ***/
    enum_<TrackableOptions>("TrackableOptions")
        .value("ARW_TRACKER_OPTION_NFT_MULTIMODE", ARW_TRACKER_OPTION_NFT_MULTIMODE)
        .value("ARW_TRACKER_OPTION_SQUARE_THRESHOLD", ARW_TRACKER_OPTION_SQUARE_THRESHOLD)
        .value("ARW_TRACKER_OPTION_SQUARE_THRESHOLD_MODE", ARW_TRACKER_OPTION_SQUARE_THRESHOLD_MODE)
        .value("ARW_TRACKER_OPTION_SQUARE_LABELING_MODE", ARW_TRACKER_OPTION_SQUARE_LABELING_MODE)
        .value("ARW_TRACKER_OPTION_SQUARE_PATTERN_DETECTION_MODE", ARW_TRACKER_OPTION_SQUARE_PATTERN_DETECTION_MODE)
        .value("ARW_TRACKER_OPTION_SQUARE_BORDER_SIZE", ARW_TRACKER_OPTION_SQUARE_BORDER_SIZE)
        .value("ARW_TRACKER_OPTION_SQUARE_MATRIX_CODE_TYPE", ARW_TRACKER_OPTION_SQUARE_MATRIX_CODE_TYPE)
        .value("ARW_TRACKER_OPTION_SQUARE_IMAGE_PROC_MODE", ARW_TRACKER_OPTION_SQUARE_IMAGE_PROC_MODE)
        .value("ARW_TRACKER_OPTION_SQUARE_DEBUG_MODE", ARW_TRACKER_OPTION_SQUARE_DEBUG_MODE)
        .value("ARW_TRACKER_OPTION_SQUARE_PATTERN_SIZE", ARW_TRACKER_OPTION_SQUARE_PATTERN_SIZE)
        .value("ARW_TRACKER_OPTION_SQUARE_PATTERN_COUNT_MAX", ARW_TRACKER_OPTION_SQUARE_PATTERN_COUNT_MAX)
        .value("ARW_TRACKER_OPTION_2D_TRACKER_FEATURE_TYPE", ARW_TRACKER_OPTION_2D_TRACKER_FEATURE_TYPE)
    ;

    function("setTrackerOptionBool", &arwSetTrackerOptionBool);
    function("setTrackerOptionInt", &arwSetTrackerOptionInt);
    function("setTrackerOptionFloat", &arwSetTrackerOptionFloat);
    function("getTrackerOptionBool", &arwGetTrackerOptionBool);
    function("arwGetTrackerOptionInt", &arwGetTrackerOptionInt);
    function("arwGetTrackerOptionFloat", &arwGetTrackerOptionFloat);

    /*** Trackable management ***/

    /**
	 * Adds a trackable as specified in the given configuration string. The format of the string can be
	 * one of:
     * - Square marker from pattern file: "single;pattern_file;pattern_width", e.g. "single;data/hiro.patt;80"
     * - Square marker from pattern passed in config: "single_buffer;pattern_width;buffer=[]", e.g. "single_buffer;80;buffer=234 221 237..."
     * - Square barcode marker: "single_barcode;barcode_id;pattern_width", e.g. "single_barcode;0;80"
     * - Multi-square marker: "multi;config_file", e.g. "multi;data/multi/marker.dat"
     * - NFT marker: "nft;nft_dataset_pathname", e.g. "nft;gibraltar"
	 * @param cfg		The configuration string
	 * @return			The unique identifier (UID) of the trackable instantiated based on the configuration string, or -1 if an error occurred
	 */
    function("addTrackable", &addTrackable);

    //TODO: To be implemented
    // value_object<ARWTrackableStatus>("ARWTrackableStatus")
    //     .field("uid", &ARWTrackableStatus::uid)
    //     .field("visible", &ARWTrackableStatus::visible)
    //     .field("matrix", &ARWTrackableStatus::matrix)
    //     .field("matrixR", &ARWTrackableStatus::matrixR)
    // ;
    // function("getTrackables", &arwGetTrackables);
    function("removeTrackable", &arwRemoveTrackable);
    function("removeAllTrackables", &arwRemoveAllTrackables);

    //** ATTENTION: arwQueryTrackableVisibilityAndTransformation is exported inside ARX_additions.js
    function("getTrackablePatternCount", &arwGetTrackablePatternCount);
    //** ATTENTION: arwGetTrackablePatternConfig is exported inside ARX_additions.js
    //** ATTENTION: arwGetTrackablePatternImage is exported inside ARX_additions.js

    enum_<TrackableOptionsSettings>("TrackableOptionsSettings")
        .value("ARW_TRACKABLE_OPTION_FILTERED", ARW_TRACKABLE_OPTION_FILTERED)
        .value("ARW_TRACKABLE_OPTION_FILTER_SAMPLE_RATE", ARW_TRACKABLE_OPTION_FILTER_SAMPLE_RATE)
        .value("ARW_TRACKABLE_OPTION_FILTER_CUTOFF_FREQ", ARW_TRACKABLE_OPTION_FILTER_CUTOFF_FREQ)
        .value("ARW_TRACKABLE_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION", ARW_TRACKABLE_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION)
        .value("ARW_TRACKABLE_OPTION_SQUARE_CONFIDENCE", ARW_TRACKABLE_OPTION_SQUARE_CONFIDENCE)
        .value("ARW_TRACKABLE_OPTION_SQUARE_CONFIDENCE_CUTOFF", ARW_TRACKABLE_OPTION_SQUARE_CONFIDENCE_CUTOFF)
        .value("ARW_TRACKABLE_OPTION_NFT_SCALE", ARW_TRACKABLE_OPTION_NFT_SCALE)
        .value("ARW_TRACKABLE_OPTION_MULTI_MIN_SUBMARKERS", ARW_TRACKABLE_OPTION_MULTI_MIN_SUBMARKERS)
        .value("ARW_TRACKABLE_OPTION_MULTI_MIN_CONF_MATRIX", ARW_TRACKABLE_OPTION_MULTI_MIN_CONF_MATRIX)
        .value("ARW_TRACKABLE_OPTION_MULTI_MIN_CONF_PATTERN", ARW_TRACKABLE_OPTION_MULTI_MIN_CONF_PATTERN)
    ;

    function("setTrackableOptionBool", &arwSetTrackableOptionBool);
    function("setTrackableOptionInt", &arwSetTrackableOptionInt);
    function("setTrackableOptionFloat", &arwSetTrackableOptionFloat);
    function("getTrackableOptionBool", &arwGetTrackableOptionBool);
    function("getTrackableOptionInt", &arwGetTrackableOptionInt);
    function("getTrackableOptionFloat", &arwGetTrackableOptionFloat);

    /*** Utility ***/

    function("teardown", &teardown);
    //** ATTENTION: arwLoadOpticalParams is exported inside ARX_additions.js
}