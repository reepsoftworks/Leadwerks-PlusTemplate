SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform mat4 projectioncameramatrix;

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
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform mat4 projectioncameramatrix;

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
}
@OpenGL4.Fragment
//from https://learnopengl.com/Advanced-Lighting/Advanced-Lighting

#version 400

//Variables
vec3 lightposition = vec3(0.5,0.5,-0.5);
vec3 light_color = vec3(0.5,1.0,1.0);
float diffuse_brightness = 1.5;
float light_brightness = 1.0;
float shininess = 64;
	
//Uniforms	
uniform sampler2D texture0;//diffuse
uniform sampler2D texture1;//normal
uniform sampler2D texture2;//specular
uniform mat4 projectionmatrix;
uniform vec3 cameraposition;

//Inputs
in vec4 ex_vertexposition;
in vec2 ex_texcoords0;
in vec3 ex_normal;
in vec3 ex_tangent;
in vec3 ex_binormal;

out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;

void main()
{

	//Diffuse 
	vec4 outcolor = texture(texture0, ex_texcoords0)* diffuse_brightness;

	//Specular 
	vec4 color_specular = texture(texture2,ex_texcoords0);

	//Normal
	vec3 normal = ex_normal;
	normal = normalize(texture(texture1,ex_texcoords0).xyz * 2.0 - 1.0);
	normal = ex_tangent * normal.x + ex_binormal * normal.y + ex_normal * normal.z;	
	normal = normalize(normal);
	
	//Blinn-Phong
	vec3 lightvec = normalize(lightposition - projectionmatrix[3].xyz);
	vec3 eyevec = normalize(cameraposition - ex_vertexposition.xyz);
	vec3 halfvec = normalize(lightvec + eyevec);
	float blinn_phong = pow(max(0.0,dot(normal, halfvec)*0.5+0.5), shininess);
	vec3 light = light_color * blinn_phong * light_brightness;
	
	//Diffuse Output
	fragData0 = mix(outcolor,vec4(light,1.0),0.5);
	
	//Normals Output
	fragData1 = vec4(normalize(normal)*0.5+0.5,fragData0.a);
	fragData1.a = 1/255.0;

	//Specular Output
	float specular = color_specular.r * 0.299 + color_specular.g * 0.587 + color_specular.b * 0.114;
	fragData2 = vec4(0,0,0,0);
}
