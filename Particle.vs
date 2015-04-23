//Mine/////////////////////////////////////////////////////////////////////////////////////
//
// Default vertex shader for materials. Extend this to deal with bump mapping, defered rendering, shadows etc.
//


// matrices
uniform mat4 modelToProjection;
uniform mat4 modelToCamera;

uniform mat4 cameraToProjection;

// attributes from vertex buffer
attribute vec4 pos;
attribute vec2 uv;
attribute vec3 normal;
attribute vec4 color;

attribute vec3 particlePos;
attribute vec4 particleColor;


// outputs
varying vec3 normal_;
varying vec2 uv_;
varying vec4 color_;
varying vec3 model_pos_;
varying vec3 camera_pos_;

void main() {

	mat4 mtpBillboard=modelToCamera;
	mtpBillboard[0][0] = 1.0; 
	mtpBillboard[0][1] = 0.0; 
	mtpBillboard[0][2] = 0.0; 

    mtpBillboard[1][0] = 0.0; 
    mtpBillboard[1][1] = 1.0; 
    mtpBillboard[1][2] = 0.0; 

	mtpBillboard[2][0] = 0.0; 
	mtpBillboard[2][1] = 0.0; 
	mtpBillboard[2][2] = 1.0; 

  gl_Position = cameraToProjection * (mtpBillboard*(pos+vec4(particlePos.xyz,0)));
  //gl_Position = modelToProjection * pos;
  vec3 tnormal = (modelToCamera * vec4(normal, 0.0)).xyz;
  vec3 tpos = (modelToCamera * pos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = particleColor;
  camera_pos_ = tpos;
  model_pos_ = pos.xyz;
}

