#version 120

attribute vec4 position;
attribute vec3 normal;
uniform mat4 mvp_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

varying vec4 color;

struct Light {
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection;
};

Light light0 = Light(
  vec4(0.0,  1.0,  2.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  vec4(1.0,  1.0,  1.0, 1.0),
  0.0, 1.0, 0.0,
  180.0, 0.0,
  vec3(0.0, 0.0, 0.0)
);
vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);
struct material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};
material mymaterial = material(
  vec4(0.2, 0.2, 0.2, 1.0),
  vec4(1.0, 0.8, 0.8, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
);

void main(void)
{
  vec3 vNormal = normal * mat3(model_matrix);
  vec4 vPosition = position * mat4(model_matrix);
  vec3 normalDirection = normalize(vNormal);
  vec3 viewDirection = normalize(vec3(view_matrix * vec4(0.0, 0.0, 0.0, 1.0) - vPosition));
  vec3 lightDirection;
  float attenuation;
 
  if (light0.position.w == 0.0) // directional light
    {
      attenuation = 1.0; // no attenuation
      lightDirection = normalize(vec3(light0.position));
    }
  else // point or spot light (or other kind of light)
    {
      vec3 vertexToLightSource = vec3(light0.position -  vPosition);
      float distance = length(vertexToLightSource);
      lightDirection = normalize(vertexToLightSource);
      attenuation = 1.0 / (light0.constantAttenuation
			   + light0.linearAttenuation * distance
			   + light0.quadraticAttenuation * distance * distance);
 
    if (light0.spotCutoff <= 90.0) // spotlight
	{
	    float clampedCosine = max(0.0, dot(-lightDirection, normalize(light0.spotDirection)));
	    if (clampedCosine < cos(radians(light0.spotCutoff))) // outside of spotlight cone
	          attenuation = 0.0;
	    else
              attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
    }
 }
  vec3 ambientLighting = vec3(scene_ambient) * vec3(mymaterial.ambient);
 
  vec3 diffuseReflection = attenuation
    * vec3(light0.diffuse) * vec3(mymaterial.diffuse)
    * max(0.0, dot(normalDirection, lightDirection));
 
  vec3 specularReflection;
  if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
      specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
  else // light source on the right side
    {
      specularReflection = attenuation * vec3(light0.specular) * vec3(mymaterial.specular)
	* pow(max(0.0, dot(reflect(lightDirection, normalDirection), viewDirection)),
	      mymaterial.shininess);
    }
 
   color = vec4(ambientLighting + diffuseReflection + specularReflection, 1.0);
   gl_Position = mvp_matrix * position;
}