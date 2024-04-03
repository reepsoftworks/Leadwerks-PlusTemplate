SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform vec4 clipplane0 = vec4(0.0);

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;

//Outputs
out vec4 ex_vertexposition;
out vec4 ex_color;
out vec2 ex_texcoords0;
out float ex_selectionstate;
//out vec3 ex_VertexCameraPosition;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;
out float clipdistance0;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;

	// Model Position
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0); // ex_vertexposition
	ex_vertexposition = entitymatrix_ * vec4(vertex_position, 1.0);
	gl_Position = projectioncameramatrix * ex_vertexposition;
	
	//Clip planes
	if (length(clipplane0.xyz)>0.0001)
	{
		clipdistance0 = modelvertexposition.x*clipplane0.x + modelvertexposition.y*clipplane0.y + modelvertexposition.z*clipplane0.z + clipplane0.w;
	}
	else
	{
		clipdistance0 = 0.0;
	}
	
	// Normals
	mat3 nmat = mat3(entitymatrix);
	ex_normal = normalize(nmat * vertex_normal);
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);

	//Texture Coordinates
	ex_texcoords0 = vertex_texcoords0;
	
	//If an object is selected, 10 is subtracted from the alpha color.
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
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
#define MAX_INSTANCES 256

//Uniforms
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;
uniform vec4 clipplane0 = vec4(0.0);

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texcoords0;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;

//Outputs
out vec4 ex_vertexposition;
out vec4 ex_color;
out vec2 ex_texcoords0;
out float ex_selectionstate;
//out vec3 ex_VertexCameraPosition;
out vec3 ex_normal;
out vec3 ex_tangent;
out vec3 ex_binormal;
out float clipdistance0;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;

	// Model Position
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0); // ex_vertexposition
	ex_vertexposition = entitymatrix_ * vec4(vertex_position, 1.0);
	gl_Position = projectioncameramatrix * ex_vertexposition;
	
	//Clip planes
	if (length(clipplane0.xyz)>0.0001)
	{
		clipdistance0 = modelvertexposition.x*clipplane0.x + modelvertexposition.y*clipplane0.y + modelvertexposition.z*clipplane0.z + clipplane0.w;
	}
	else
	{
		clipdistance0 = 0.0;
	}
	
	// Normals
	mat3 nmat = mat3(entitymatrix);
	ex_normal = normalize(nmat * vertex_normal);
	ex_tangent = normalize(nmat * vertex_tangent);
	ex_binormal = normalize(nmat * vertex_binormal);

	//Texture Coordinates
	ex_texcoords0 = vertex_texcoords0;
	
	//If an object is selected, 10 is subtracted from the alpha color.
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	ex_color = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
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

// Blinn-Phong
vec3 lightposition = vec3(0.5,0.5,-0.5);
float diffuse_brightness = 1.8;
float shininess = 64;

//Uniforms
uniform sampler2D texture0;//diffuse map
uniform sampler2D texture2;//specular map
uniform samplerCube texture15;//BFN map
uniform vec4 materialcolorspecular;
uniform int decalmode;
uniform float materialroughness;
uniform mat4 projectionmatrix;
uniform vec3 cameraposition;

//Inputs
in vec2 ex_texcoords0;
in vec4 ex_color;
in float ex_selectionstate;
//in vec3 ex_VertexCameraPosition;
in vec4 ex_vertexposition;
in vec3 ex_normal;
in vec3 ex_tangent;
in vec3 ex_binormal;
in float clipdistance0;

//Outputs
out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;
out vec4 fragData3;

void main(void)
{
	//Clip plane discard
	if (clipdistance0>0.0) discard;
	
	vec4 outcolor = ex_color;
	vec3 normal = ex_normal;

	//Modulate blend with diffuse map
	outcolor *= texture(texture0,ex_texcoords0) * diffuse_brightness;

	// Blinn-Phong
	vec3 light_color = vec3(materialcolorspecular.r,materialcolorspecular.g,materialcolorspecular.b);
	float light_brightness = materialcolorspecular.a;
	vec3 lightvec = normalize(lightposition - projectionmatrix[3].xyz);
	vec3 eyevec = normalize(cameraposition - ex_vertexposition.xyz);
	vec3 halfvec = normalize(lightvec + eyevec);
	float blinn_phong = pow(max(0.0,dot(normal, halfvec)*0.5+0.5), shininess);
	vec3 light = light_color * blinn_phong * light_brightness;	

	//Blend with selection color if selected
	fragData0 = mix(outcolor,vec4(light,1.0),0.5);
	
#if BFN_ENABLED==1
	//Best-fit normals
	fragData1 = texture(texture15,normalize(vec3(normal.x,-normal.y,normal.z)));
	fragData1.a = fragData0.a;
#else
	//Low-res normals
	fragData1 = vec4(normalize(normal)*0.5+0.5,1.0);
#endif

	// Specular
	vec4 color_specular = texture(texture2,ex_texcoords0) * materialcolorspecular;
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

	float reflectlight = 1.0F - materialroughness;
	fragData1.a = materialflags/255.0;
	fragData2 = vec4(0.0,0.0,0.0,reflectlight);
	fragData3 = ex_vertexposition;
}
