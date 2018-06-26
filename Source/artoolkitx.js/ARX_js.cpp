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
    constant("arController", &gARTK);

    function("setLogLevel", &arwSetLogLevel);

    /*** artoolkitX lifecycle functions ***/
    function("initialiseAR", &arwInitialiseAR);
    function("getARToolKitVersion", &getARToolKitVersion);
    function("getError", &arwGetError);
    
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
    //** ATTENTION: arwGetTrackablePatternConfig is exported inside append.js
    //** ATTENTION: arwGetTrackablePatternImage is exported inside append.js

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

    function("setTrackableOptionBool", arwSetTrackableOptionBool);
    function("setTrackableOptionInt", arwSetTrackableOptionInt);
    function("setTrackableOptionFloat", arwSetTrackableOptionFloat);
    function("getTrackableOptionBool", arwGetTrackableOptionBool);
    function("getTrackableOptionInt", arwGetTrackableOptionInt);
    function("getTrackableOptionFloat", arwGetTrackableOptionFloat);

    /*** Utility ***/

    //** ATTENTION: arwLoadOpticalParams is exported inside append.js
}