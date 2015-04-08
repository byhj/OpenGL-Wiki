#version 120

attribute vec4 position;
attribute vec3 normal;
uniform mat4 mvp_matrix;
uniform mat4 model_matrix;

varying vec3 color;

struct Light {
  vec4 position;
  vec4 diffuse;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};

Light light0 = Light(
  vec4(0.0, 1.0, 2.0, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  0.0, 1.0, 0.0,
  80.0, 20.0,
  vec3(-1.0, -0.5, -1.0)
);

struct Material {
  vec4 diffuse;
};

Material material = Material(vec4(1.0, 0.8, 0.8, 1.0));

void main(void)
{
  vec3 normalDir = normalize(normal * mat3(model_matrix)); //法线方向
  vec4 vPosition = position * mat4(model_matrix);
  float attenuation; //衰减
  vec3 lightDir;   //光线法线

  if (light0.position.w == 0.0) { //方向光
     attenuation = 1.0;
     lightDir = normalize(vec3(light0.position));
  } 
  else {        //点光源或者聚光灯
       vec3 vertexToLight = vec3(light0.position - vPosition);
	   lightDir = normalize(vertexToLight);
	   float distance = length(vertexToLight); //光源距离模型观察点的长度
	   attenuation = 1.0 / (light0.constantAttenuation 
	                        + light0.linearAttenuation * distance
	                        + light0.quadraticAttenuation * distance * distance);

      if (light0.spotCutoff <= 90.0) {
          float clampedCosine = max(0.0, dot(-lightDir, normalize(light0.spotDirection)));
          if (clampedCosine < cos(light0.spotCutoff * 3.14159 / 180.0)) // outside of spotlight cone
              attenuation = 0.0;
	      else
	         attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
     }
  }
      
  vec3 diffuseReflection = attenuation
                          * vec3(light0.diffuse) * vec3(material.diffuse)
                          * max(0.0, dot(normalDir, lightDir));
 
  color = diffuseReflection;
  gl_Position = mvp_matrix * position;
}