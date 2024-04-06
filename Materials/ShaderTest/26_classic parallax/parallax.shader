SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform mat4 projectioncameramatrix;
uniform vec3 cameraposition;

//Attributes
in vec3 vertex_position;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec3 vertex_binormal;

//Outputs
out vec4 ex_vertexposition;
out vec2 ex_texcoords0;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;

out vec3 eyevec;

void main()
  {
  
	//entity matrix
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_ = entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	//model position
	ex_vertexposition = entitymatrix_ * vec4(vertex_position, 1.0);
	gl_Position = projectioncameramatrix * ex_vertexposition;

	//normals
	mat3 nmat = mat3(entitymatrix_);
	ex_normal = normalize(nmat * vertex_normal);	
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);

	//texture coordinates
	ex_texcoords0 = vertex_texcoords0;

	//TBN matrix
	//--------------------------------------------------------------
	mat3 TBNMatrix = mat3(ex_tangent, ex_binormal, ex_normal);
    eyevec = cameraposition.xyz - ex_vertexposition.xyz;
    eyevec *= TBNMatrix;
	//--------------------------------------------------------------
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform mat4 projectioncameramatrix;
uniform vec3 cameraposition;

//Attributes
in vec3 vertex_position;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_tangent;
in vec3 vertex_binormal;

//Outputs
out vec4 ex_vertexposition;
out vec2 ex_texcoords0;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;

out vec3 eyevec;

void main()
  {
  
	//entity matrix
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_ = entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	//model position
	ex_vertexposition = entitymatrix_ * vec4(vertex_position, 1.0);
	gl_Position = projectioncameramatrix * ex_vertexposition;

	//normals
	mat3 nmat = mat3(entitymatrix_);
	ex_normal = normalize(nmat * vertex_normal);	
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);

	//texture coordinates
	ex_texcoords0 = vertex_texcoords0;

	//TBN matrix
	//--------------------------------------------------------------
	mat3 TBNMatrix = mat3(ex_tangent, ex_binormal, ex_normal);
    eyevec = cameraposition.xyz - ex_vertexposition.xyz;
    eyevec *= TBNMatrix;
	//--------------------------------------------------------------
}
@OpenGL4.Fragment
//from https://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

#version 400

// Variables
float scale = 0.04;
float bias = 0.02; 

//Uniforms	
uniform sampler2D texture0;//diffuse
uniform sampler2D texture1;//normal
uniform sampler2D texture2;//specular
uniform sampler2D texture3;//height

//Inputs
in vec4 ex_vertexposition;
in vec2 ex_texcoords0;
in vec3 ex_normal;
in vec3 ex_tangent;
in vec3 ex_binormal;
in vec3 eyevec;

out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;

void main() {

	//Height
	//------------------------------------------------------------------------
	float height = texture(texture3, ex_texcoords0).r;
	float v = height * scale - bias;
	vec3 eye = normalize(eyevec);
	vec2 newCoords = mix(ex_texcoords0, ex_texcoords0 + (eye.xy * v), 0.5);
	//------------------------------------------------------------------------

	//Diffuse 
	vec4 outcolor = texture(texture0, newCoords);

	//Specular 
	vec4 color_specular = texture(texture2,newCoords);

	//Normal
	vec3 normal = ex_normal;
	normal = normalize(texture(texture1,newCoords).xyz * 2.0 - 1.0);
	normal.z /= 2.0;
	normal = ex_tangent * normal.x + ex_binormal * normal.y + ex_normal * normal.z;	
	normal = normalize(normal);

	//Diffuse Output
    fragData0 = outcolor;
	
	//Normals Output
	fragData1 = vec4(normalize(normal)*0.5+0.5,fragData0.a);
	fragData1.a = 1/255.0;

	//Specular Output
	float specular = color_specular.r * 0.299 + color_specular.g * 0.587 + color_specular.b * 0.114;
	fragData2 = vec4(0,0,0,specular);
}
