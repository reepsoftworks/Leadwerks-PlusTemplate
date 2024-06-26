SHADER version 1
@OpenGL2.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
//uniform mat4 entitymatrix;
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;
in vec2 vertex_texcoords0;

//Outputs
//out vec4 ex_color;
out float ex_selectionstate;
//out vec3 ex_VertexCameraPosition;
//out vec3 ex_normal;
//out vec2 ex_texcoords0;

//Tessellation
out vec4 vPosition;
out vec2 vTexCoords0;
out vec3 vNormal;
out vec3 vBinormal;
out vec3 vTangent;
out vec4 vColor;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	vColor=vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	//ex_VertexCameraPosition = vec3(camerainversematrix * modelvertexposition);
	vPosition = modelvertexposition;
	gl_Position = projectioncameramatrix * vPosition;
	
	//ctrl_transformmatrix = projectioncameramatrix * entitymatrix_;
	
	//ex_VertexCameraPosition = vec3(camerainversematrix * vec4(vertex_position, 1.0));
	//gl_Position = projectioncameramatrix * entitymatrix_ * vec4(vertex_position, 1.0);
	
	mat3 nmat = mat3(camerainversematrix[0].xyz,camerainversematrix[1].xyz,camerainversematrix[2].xyz);//39
	nmat = nmat * mat3(entitymatrix[0].xyz,entitymatrix[1].xyz,entitymatrix[2].xyz);//40
	vNormal = normalize(nmat * vertex_normal);
	vBinormal = normalize(nmat * vertex_binormal);	
	vTangent = normalize(nmat * vertex_tangent);		
	
	vTexCoords0 = vertex_texcoords0;
	
	vColor = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	//If an object is selected, 10 is subtracted from the alpha color.
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	
	ex_selectionstate = 0.0;
	if (vColor.a<-5.0)
	{
		vColor.a += 10.0;
		ex_selectionstate = 1.0;
	}
	vColor *= vec4(1.0-vertex_color.r,1.0-vertex_color.g,1.0-vertex_color.b,vertex_color.a) * materialcolordiffuse;
	
	//Tessellation
	//vPosition = entitymatrix_ * vec4(vertex_position,1.0);
	//ctrl_normal = ex_normal;	
	//ctrl_color = ex_color;
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400
#define MAX_INSTANCES 256

//Uniforms
//uniform mat4 entitymatrix;
uniform vec4 materialcolordiffuse;
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform instancematrices { mat4 matrix[MAX_INSTANCES];} entity;

//Attributes
in vec3 vertex_position;
in vec4 vertex_color;
in vec3 vertex_normal;
in vec3 vertex_binormal;
in vec3 vertex_tangent;
in vec2 vertex_texcoords0;

//Outputs
//out vec4 ex_color;
out float ex_selectionstate;
//out vec3 ex_VertexCameraPosition;
//out vec3 ex_normal;
//out vec2 ex_texcoords0;

//Tessellation
out vec4 vPosition;
out vec2 vTexCoords0;
out vec3 vNormal;
out vec3 vBinormal;
out vec3 vTangent;
out vec4 vColor;

void main()
{
	mat4 entitymatrix = entity.matrix[gl_InstanceID];
	mat4 entitymatrix_=entitymatrix;
	entitymatrix_[0][3]=0.0;
	entitymatrix_[1][3]=0.0;
	entitymatrix_[2][3]=0.0;
	entitymatrix_[3][3]=1.0;
	
	vColor=vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	vec4 modelvertexposition = entitymatrix_ * vec4(vertex_position,1.0);
	//ex_VertexCameraPosition = vec3(camerainversematrix * modelvertexposition);
	vPosition = modelvertexposition;
	gl_Position = projectioncameramatrix * vPosition;
	
	//ctrl_transformmatrix = projectioncameramatrix * entitymatrix_;
	
	//ex_VertexCameraPosition = vec3(camerainversematrix * vec4(vertex_position, 1.0));
	//gl_Position = projectioncameramatrix * entitymatrix_ * vec4(vertex_position, 1.0);
	
	mat3 nmat = mat3(camerainversematrix[0].xyz,camerainversematrix[1].xyz,camerainversematrix[2].xyz);//39
	nmat = nmat * mat3(entitymatrix[0].xyz,entitymatrix[1].xyz,entitymatrix[2].xyz);//40
	vNormal = normalize(nmat * vertex_normal);
	vBinormal = normalize(nmat * vertex_binormal);	
	vTangent = normalize(nmat * vertex_tangent);		
	
	vTexCoords0 = vertex_texcoords0;
	
	vColor = vec4(entitymatrix[0][3],entitymatrix[1][3],entitymatrix[2][3],entitymatrix[3][3]);
	
	//If an object is selected, 10 is subtracted from the alpha color.
	//This is a bit of a hack that packs a per-object boolean into the alpha value.
	
	ex_selectionstate = 0.0;
	if (vColor.a<-5.0)
	{
		vColor.a += 10.0;
		ex_selectionstate = 1.0;
	}
	vColor *= vec4(1.0-vertex_color.r,1.0-vertex_color.g,1.0-vertex_color.b,vertex_color.a) * materialcolordiffuse;
	
	//Tessellation
	//vPosition = entitymatrix_ * vec4(vertex_position,1.0);
	//ctrl_normal = ex_normal;	
	//ctrl_color = ex_color;
}
@OpenGL4.Fragment
#version 400
#define BFN_ENABLED 1

//Uniforms
uniform vec4 materialcolordiffuse;
uniform float currenttime;
uniform int lightingmode;
uniform vec2 buffersize;
uniform vec2 camerarange;
uniform float camerazoom;
uniform vec4 materialcolorspecular;
uniform vec4 lighting_ambient;
uniform sampler2D texture0;// reflection
uniform sampler2D texture1;// normal
uniform sampler2D texture2;
uniform sampler2D texture3;// refraction
uniform sampler2D texture4;// depth
uniform samplerCube texture15;
uniform float blend;
uniform bool isbackbuffer;
uniform vec4 camerawaterplane;
uniform mat3 camerainversenormalmatrix;
uniform mat3 cameranormalmatrix;

//Fog parameters
uniform bool fogmode = true;
uniform vec4 fogcolor = vec4(1.0);
uniform vec2 fogrange = vec2(0.0,100.0);
uniform vec2 fogangle = vec2(90.0,90.0);

//Inputs
in float ex_selectionstate;
in vec2 vTexCoords0;
in vec3 vNormal;
in vec3 vBinormal;
in vec3 vTangent;
in vec4 vColor;
in vec4 vPosition;

//Outputs
out vec4 fragData0;
out vec4 fragData1;
out vec4 fragData2;
out vec4 fragData3;

uniform float amplitude = 0.1; //0.05;

float depthToPosition(in float depth, in vec2 depthrange)
{
	return depthrange.x / (depthrange.y - depth * (depthrange.y - depthrange.x)) * depthrange.y;
}

void main(void)
{
	vec4 outcolor = vColor;
	
	float frame = currenttime/100000.0;
	vec3 p0 = texture(texture1,vPosition.xz/4.0).xyz*2.0-1.0;
	vec3 p1 = texture(texture2,vPosition.xz/4.0).xyz*2.0-1.0;
	
	/*p0 += texture(texture1,vPosition.xz/40.0).xyz*2.0-1.0;
	p1 += texture(texture2,vPosition.xz/40.0).xyz*2.0-1.0;
	*/
	vec3 perturbation = normalize(p0*(1.0-blend) + p1*blend);
	//perturbation.z*=0.25;
	//perturbation=normalize(perturbation);
	
	vec3 sundir = vec3(0.707,-0.707,0.707);
	float brightness = -dot(perturbation.xzy,sundir);
	brightness = max(0.0,brightness)*0.75+0.25;
	
	perturbation.z=0.0;
	
	vec3 screencoord = vec3(gl_FragCoord.x/buffersize.x,gl_FragCoord.y/buffersize.y,1.0);	
	if (isbackbuffer) screencoord.y = 1.0 - screencoord.y;
	vec3 reflectionvector = screencoord;
	
	//reflectionvector=normalize(reflectionvector+perturbation*amplitude);
	
	//Reflection
	vec4 reflection = textureProj(texture0,(reflectionvector+perturbation*amplitude));
	
	//perturbation = camerainversenormalmatrix * perturbation;
	
	float depth = textureProj(texture4,(reflectionvector+perturbation*amplitude)).r;
	
	//Refraction
	if (gl_FragCoord.z>depth)
	{
		perturbation = vec3(0.0);
		depth = textureProj(texture4,reflectionvector).r;
	}
	
	float z = depthToPosition(depth,camerarange);
	z = z - depthToPosition(gl_FragCoord.z,camerarange);
	
	vec4 refraction = textureProj(texture3,(reflectionvector+perturbation*amplitude));
	
	float rendermix = 0.5;
	
	vec3 screennormal;
	screencoord = vec3(((gl_FragCoord.x/buffersize.x)-0.5) * 2.0 * (buffersize.x/buffersize.y),((-gl_FragCoord.y/buffersize.y)+0.5) * 2.0,depthToPosition(gl_FragCoord.z,camerarange));
	screencoord.x *= screencoord.z / camerazoom;
	screencoord.y *= -screencoord.z / camerazoom;
	if (!isbackbuffer) screencoord.y *= -1.0;	
	screennormal = normalize(screencoord);
	
	rendermix = -dot(screennormal,vec3(vNormal.x,vNormal.y,vNormal.z));
	//rendermix = clamp(rendermix,0.0,1.0);
	
	vec3 normal;

/*
	#define SHALLOWRANGE 0.0f
	
	if (z>SHALLOWRANGE)
	{
		float depthmix = min(1.0,(z-SHALLOWRANGE)/(10.0-SHALLOWRANGE));
		depthmix=sqrt(depthmix);
		refraction = refraction * (1.0-depthmix) + (depthmix) * vColor;
	}
	
	outcolor = refraction*rendermix + reflection*(1.0-rendermix);
	
	outcolor *= brightness;
	
	if (z<SHALLOWRANGE)
	{
		float depthmix = z / SHALLOWRANGE;
		depthmix=pow(depthmix,1.0/2.0);
		outcolor = refraction * (1.0-depthmix) + (depthmix) * outcolor;
	}
*/
	
	outcolor = reflection*(1.0-rendermix);
	
	outcolor *= vColor+brightness;
	
	
	//-------------------------------------------------
	//Fog
	vec3 worldpos = cameranormalmatrix * screencoord;
	vec3 cubecoord = normalize(worldpos.xyz);
	float fogeffect = 0.0f;
	
	if (fogmode==true)
	{
		fogeffect = clamp( 1.0 - (fogrange.y - length(screencoord.xyz)) / (fogrange.y - fogrange.x) , 0.0, 1.0 );
		fogeffect*=fogcolor.a;
	}
	//--------------------------------------------------------

	vec4 specular;
	normal = cameranormalmatrix * -normalize(perturbation * vec3(1,0.01,1));
	vec3 lightdirection = vNormal;
	vec3 lightreflection = normalize(reflect(lightdirection,normal));
	const float gloss = 1.0f;
	specular = vec4( pow(clamp(dot(lightreflection,screennormal),0.0,1.0),gloss) * 0.25);
	
	//-------------------------------------------------------------------------------------------
	
	//outcolor = vec4(rendermix);
	outcolor.a = 1.0;//vColor.a;	
	fragData0 = vec4(fogcolor.xyz,1.0)*fogeffect + outcolor*(1.0-fogeffect);
	
	//Blend with selection color if selected
	normal=vNormal;
	
#if BFN_ENABLED==1
	//Best-fit normals
	fragData1 = texture(texture15,normalize(vec3(normal.x,-normal.y,normal.z)));
	fragData1.a = fragData0.a;
#else
	//Low-res normals
	fragData1 = vec4(normalize(normal)*0.5+0.5,fragData0.a);
#endif
	fragData1.a = materialcolorspecular.r * 0.299 + materialcolorspecular.g * 0.587 + materialcolorspecular.b * 0.114;
	int materialflags=1;
	if (ex_selectionstate>0.0) materialflags += 2;
	fragData2 = vec4(0.0,0.0,0.0,materialflags/255.0);
}
