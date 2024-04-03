SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform mat4 projectioncameramatrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;

//Attributes
in vec3 vertex_position;
in vec2 vertex_texcoords0;

out vec2 ex_texcoords0;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	ex_texcoords0 = vertex_texcoords0;

	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	gl_Position = projectioncameramatrix * modelvertexposition;
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform mat4 projectioncameramatrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;

//Attributes
in vec3 vertex_position;
in vec2 vertex_texcoords0;

out vec2 ex_texcoords0;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	ex_texcoords0 = vertex_texcoords0;

	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	gl_Position = projectioncameramatrix * modelvertexposition;
}
@OpenGL4.Fragment
#version 400

uniform sampler2D texture0;

in vec2 ex_texcoords0;

void main()
{
	if (texture(texture0,ex_texcoords0).a<0.5) discard;
}
