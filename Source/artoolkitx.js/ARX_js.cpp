/**
 *  
 * 
 */

#include <emscripten/bind.h>
#include "ARX/ARX_c.h"

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

int addTrackable(std::string cfg) {
    return arwAddTrackable(cfg.c_str());
}

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

//TODO: to be implemente
// bool getTrackables() {
//     int count;
//     ARWTrackableStatus status;
//     arwGetTrackables(&count, status)
// }

EMSCRIPTEN_BINDINGS(constant_bindings) {

/*
	function("setup", &setup);
	function("teardown", &teardown);

	function("_addMarker", &addMarker);
	function("_addMultiMarker", &addMultiMarker);

	function("getMultiMarkerNum", &getMultiMarkerNum);
	function("getMultiMarkerCount", &getMultiMarkerCount);

	function("_loadCamera", &loadCamera);

	function("setMarkerInfoDir", &setMarkerInfoDir);
	function("setMarkerInfoVertex", &setMarkerInfoVertex);

	function("getTransMatSquare", &getTransMatSquare);
	function("getTransMatSquareCont", &getTransMatSquareCont);

	function("getTransMatMultiSquare", &getTransMatMultiSquare);
	function("getTransMatMultiSquareRobust", &getTransMatMultiSquareRobust);

	function("detectMarker", &detectMarker);
	function("getMarkerNum", &getMarkerNum);

	function("getMultiEachMarker", &getMultiEachMarkerInfo);
	function("getMarker", &getMarkerInfo);
*/

	/* AR Toolkit C APIS */
	// function("setDebugMode", &setDebugMode);
	// function("getDebugMode", &getDebugMode);

	// function("getProcessingImage", &getProcessingImage);


    /*** artoolkitX lifecycle functions ***/
    function("setLogLevel", &arwSetLogLevel);
    function("initialiseAR", &arwInitialiseAR);
    function("getARToolKitVersion", &getARToolKitVersion);
    function("isRunning", &arwIsRunning);
    function("isInitialized", &arwIsInited);
    function("stopRunning", &arwStopRunning);
    function("shutdownAR", &arwShutdownAR);

    /*** Video stream management ***/
    //* ATTENTION: arwGetProjectionMatrix is exported from append.js

    value_object<VideoParams>("VideoParams")
        .field("width", &VideoParams::width)
        .field("height", &VideoParams::height)
        .field("pixelSize", &VideoParams::pixelSize)
        .field("pixelFormat", &VideoParams::pixelFormat);
    function("getVideoParams", &getVideoParams);

    function("capture", &arwCapture);
    function("updateAR", &arwUpdateAR);

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

    function("addTrackable", &addTrackable);

    //TODO: To be implemented
    // value_object<ARWTrackableStatus>("ARWTrackableStatus")
    //     .field("uid", &ARWTrackableStatus::uid)
    //     .field("visible", &ARWTrackableStatus::visible)
    //     .field("matrix", &ARWTrackableStatus::matrix)
    //     .field("matrixR", &ARWTrackableStatus::matrixR)
    // ;
    // function("arwGetTrackables", &arwGetTrackables);
    function("removeTrackable", &arwRemoveTrackable);
    function("removeAllTrackables", &arwRemoveAllTrackables);

    //** ATTENTION: arwQueryTrackableVisibilityAndTransformation is exported inside append.js
    function("getTrackablePatternCount", &arwGetTrackablePatternCount);
    

	// function("getLogLevel", &getLogLevel);

	// function("setProjectionNearPlane", &setProjectionNearPlane);
	// function("getProjectionNearPlane", &getProjectionNearPlane);

	// function("setProjectionFarPlane", &setProjectionFarPlane);
	// function("getProjectionFarPlane", &getProjectionFarPlane);

	// function("setThresholdMode", &setThresholdMode);
	// function("getThresholdMode", &getThresholdMode);

	// function("setThreshold", &setThreshold);
	// function("getThreshold", &getThreshold);

	// function("setPatternDetectionMode", &setPatternDetectionMode);
	// function("getPatternDetectionMode", &getPatternDetectionMode);

	// function("setPattRatio", &setPattRatio);
	// function("getPattRatio", &getPattRatio);

	// function("setMatrixCodeType", &setMatrixCodeType);
	// function("getMatrixCodeType", &getMatrixCodeType);

	// function("setLabelingMode", &setLabelingMode);
	// function("getLabelingMode", &getLabelingMode);

	// function("setImageProcMode", &setImageProcMode);
	// function("getImageProcMode", &getImageProcMode);

}