SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_BONES 256

//Uniforms
uniform mat4 entitymatrix;
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform bonematrices { mat4 matrix[MAX_BONES];} bone;
uniform vec4 clipplane0 = vec4(0.0);

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;
in vec4 vertex_boneweights;
in ivec4 vertex_boneindices;

//Outputs
out vec4 ex_color;
out vec2 ex_texcoords0;
out float ex_selectionstate;
out vec3 ex_VertexCameraPosition;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;
out float clipdistance0;

void main()
{
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;

	vec4 wt = vertex_boneweights;
	float m = wt[0]+wt[1]+wt[2]+wt[3];
	wt[0]/=m; wt[1]/=m; wt[2]/=m; wt[3]/=m;
	
	mat4 animmatrix = bone.matrix[vertex_boneindices[0]] * wt[0];
	animmatrix += bone.matrix[vertex_boneindices[1]] * wt[1];
	animmatrix += bone.matrix[vertex_boneindices[2]] * wt[2];
	animmatrix += bone.matrix[vertex_boneindices[3]] * wt[3];	
	
	animmatrix[0][3]=0.0;
	animmatrix[1][3]=0.0;
	animmatrix[2][3]=0.0;
	animmatrix[3][3]=1.0;
	
	entitymatrix_ *= animmatrix;
	
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	
	//Clip planes
	if (length(clipplane0.xyz)>0.0001)
	{
		clipdistance0 = modelvertexposition.x*clipplane0.x + modelvertexposition.y*clipplane0.y + modelvertexposition.z*clipplane0.z + clipplane0.w;
	}
	else
	{
		clipdistance0 = 0.0;
	}
	
	ex_VertexCameraPosition = vec3(camerainversematrix * modelvertexposition);
	gl_Position = projectioncameramatrix * modelvertexposition;
	
	mat3 nmat = mat3(entitymatrix_[0].xyz,entitymatrix_[1].xyz,entitymatrix_[2].xyz);//40
	ex_normal = normalize(nmat * vertex_normal);	
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);
	
	ex_texcoords0 = vertex_texcoords0;
	
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	ex_selectionstate = 0.0;
	if (ex_color.a<-5.0)
	{
		ex_color.a += 10.0;
		ex_selectionstate = 1.0;
	}
	ex_color *= vec4(1.0-vertex_color.r,1.0-vertex_color.g,1.0-vertex_color.b,vertex_color.a) * materialcolordiffuse;
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400
#define MAX_BONES 256

//Uniforms
uniform mat4 entitymatrix;
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform bonematrices { mat4 matrix[MAX_BONES];} bone;
uniform vec4 clipplane0 = vec4(0.0);

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;
in vec4 vertex_boneweights;
in ivec4 vertex_boneindices;

//Outputs
out vec4 ex_color;
out vec2 ex_texcoords0;
out float ex_selectionstate;
out vec3 ex_VertexCameraPosition;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;
out float clipdistance0;

void main()
{
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;

	vec4 wt = vertex_boneweights;
	float m = wt[0]+wt[1]+wt[2]+wt[3];
	wt[0]/=m; wt[1]/=m; wt[2]/=m; wt[3]/=m;
	
	mat4 animmatrix = bone.matrix[vertex_boneindices[0]] * wt[0];
	animmatrix += bone.matrix[vertex_boneindices[1]] * wt[1];
	animmatrix += bone.matrix[vertex_boneindices[2]] * wt[2];
	animmatrix += bone.matrix[vertex_boneindices[3]] * wt[3];	
	
	animmatrix[0][3]=0.0;
	animmatrix[1][3]=0.0;
	animmatrix[2][3]=0.0;
	animmatrix[3][3]=1.0;
	
	entitymatrix_ *= animmatrix;
	
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	
	//Clip planes
	if (length(clipplane0.xyz)>0.0001)
	{
		clipdistance0 = modelvertexposition.x*clipplane0.x + modelvertexposition.y*clipplane0.y + modelvertexposition.z*clipplane0.z + clipplane0.w;
	}
	else
	{
		clipdistance0 = 0.0;
	}
	
	ex_VertexCameraPosition = vec3(camerainversematrix * modelvertexposition);
	gl_Position = projectioncameramatrix * modelvertexposition;
	
	mat3 nmat = mat3(entitymatrix_[0].xyz,entitymatrix_[1].xyz,entitymatrix_[2].xyz);//40
	ex_normal = normalize(nmat * vertex_normal);	
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);
	
	ex_texcoords0 = vertex_texcoords0;
	
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	ex_selectionstate = 0.0;
	if (ex_color.a<-5.0)
	{
		ex_color.a += 10.0;
		ex_selectionstate = 1.0;
	}
	ex_color *= vec4(1.0-vertex_color.r,1.0-vertex_color.g,1.0-vertex_color.b,vertex_color.a) * materialcolordiffuse;
}
@OpenGL4.Fragment
#version 400
#define BFN_ENABLED 1

//Uniforms
uniform vec4 materialcolorspecular;
uniform vec4 lighting_ambient;
uniform samplerCube texture15;
uniform vec2 camerarange;
uniform vec2 buffersize;
uniform float camerazoom;
uniform int decalmode;
uniform float materialroughness;

//Inputs
in vec2 ex_texcoords0;
in vec4 ex_color;
in float ex_selectionstate;
in vec3 ex_VertexCameraPosition;
in vec3 ex_normal;
in vec3 ex_tangent;
in vec3 ex_binormal;
in float clipdistance0;

//Outputs
out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;
out vec4 fragData3;

float DepthToZPosition(in float depth) {
	return camerarange.x / (camerarange.y - depth * (camerarange.y - camerarange.x)) * camerarange.y;
}

void main(void)
{
	//Clip plane discard
	if (clipdistance0>0.0) discard;
	
	vec4 outcolor = ex_color;
	vec4 color_specular = materialcolorspecular;
	
	vec3 screencoord = vec3(((gl_FragCoord.x/buffersize.x)-0.5) * 2.0 * (buffersize.x/buffersize.y),((-gl_FragCoord.y/buffersize.y)+0.5) * 2.0,DepthToZPosition( gl_FragCoord.z ));
	screencoord.x *= screencoord.z / camerazoom;
	screencoord.y *= -screencoord.z / camerazoom;   
	vec3 nscreencoord = normalize(screencoord);
	
	//Modulate blend with diffuse map
	
	//Normal map
	vec3 normal = normalize(ex_normal);
	
	//Calculate lighting
	vec4 lighting_diffuse = vec4(0);
	vec4 lighting_specular = vec4(0);
	float attenuation=1.0;
	vec3 lightdir;
	vec3 lightreflection;
	int i;
	float anglecos;
	float diffspotangle;	
	float denom;
	
	//Blend with selection color if selected
	fragData0 = outcolor;

#if BFN_ENABLED==1
	//Best-fit normals
	fragData1 = texture(texture15,normalize(vec3(normal.x,-normal.y,normal.z)));
	fragData1.a = fragData0.a;
#else
	//Low-res normals
	fragData1 = vec4(normalize(normal)*0.5+0.5,fragData0.a);
#endif
	float specular = color_specular.r * 0.299 + color_specular.g * 0.587 + color_specular.b * 0.114;
	int materialflags=1;
	if (ex_selectionstate>0.0) materialflags += 2;
	if (decalmode==1) materialflags += 4;//brush
	if (decalmode==2) materialflags += 8;//model
	if (decalmode==4) materialflags += 16;//terrain
	if (materialroughness>=0.5)
	{
		materialflags += 32;
		if (materialroughness>=0.75) materialflags += 64;
	}
	else
	{
		if (materialroughness>=0.25) materialflags += 64;
	}
	fragData1.a = materialflags/255.0;
	fragData2 = vec4(0.0,0.0,0.0,specular);
}
