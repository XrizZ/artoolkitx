#include <emscripten/bind.h>
#include "ARX/ARX_c.h"

using namespace emscripten;

std::string getARToolKitVersion()
{
    char versionString[1024];
    std::string returnValue ("unknown version");
    if (arwGetARToolKitVersion(versionString, 1024)){
        return std::string(versionString);
    }
    return returnValue;
}

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

    function("setLogLevel", &arwSetLogLevel);
    function("initialiseAR", &arwInitialiseAR);
    function("getARToolKitVersion", &getARToolKitVersion);

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