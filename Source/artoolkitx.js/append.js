// Many thanks to https://github.com/Planeshifter for his project: https://github.com/Planeshifter/emscripten-examples/tree/master/01_PassingArrays

function _arrayFloatToHeap(typedArray){
    var numBytes = typedArray.length * typedArray.BYTES_PER_ELEMENT;
    var ptr = Module._malloc(numBytes);
    var heapBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, numBytes);
    heapBytes.set(new Uint8Array(typedArray.buffer));
    return heapBytes;
  }

Module["getProjectionMatrix"] = function(nearPlane, farPlane){
    var projectionMatrix = new Float32Array(16);
    var heapBytes = _arrayFloatToHeap(projectionMatrix);

    if( ! Module.ccall('arwGetProjectionMatrix', 'boolean',['number','number','number'], [nearPlane, farPlane, heapBytes.byteOffset])) {
        return undefined;
    }
    var returnValue = new Float32Array(heapBytes);
    Module._free(heapBytes.byteOffset);

    return returnValue;
   };

Module["queryTrackableVisibilityAndTransformation"] = function (trackableId) {
    var matrix = new Float32Array(16);
    var heapBytes = _arrayFloatToHeap(matrix);

    if( ! Module.ccall('arwQueryTrackableVisibilityAndTransformation','boolean',['number', 'number'], [trackableId, heapBytes])) {
        return false;
    }
    matrix = new Float32Array(heapBytes);
    Module._free(heapBytes.byteOffset);

    return returnValue;
}

Module["getTrackablePatternConfig"] = function (trackableId, patternID) {
    var heapBytes = _arrayFloatToHeap(new Float32Array(16));
    var widthHeapBytes = _arrayFloatToHeap(new Float32Array(1));
    var heightHeapBytes = _arrayFloatToHeap(new Float32Array(1));
    var sizeXHeapBytes = _arrayFloatToHeap(new Float32Array(1));
    var sizeYHeapBytes = _arrayFloatToHeap(new Float32Array(1));
    if( !Module.ccall('arwGetTrackablePatternConfig','boolean',['number','number','number','number','number','number','number','number'], [trackableId, patternID, heapBytes, widthHeapBytes, heightHeapBytes, sizeXHeapBytes, sizeYHeapBytes])){
        return undefined;
    }
    
    var returnObject = {
        matrix: new Float32Array(heapBytes),
        width: new Float32Array(widthHeapBytes)[0],
        height: new Float32Array(heightHeapBytes)[0],
        sizeX: new Float32Array(sizeXHeapBytes)[0],
        sizeY: new Float32Array(sizeXHeapBytes)[0]
    };
    return returnObject;
    
}


