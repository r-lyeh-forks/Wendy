
attribute vec3 wyPosition;
attribute vec3 wyNormal;

varying vec3 eyepos;
varying vec3 normal;

void main()
{
  eyepos = (wyMV * vec4(wyPosition, 1)).xyz;
  normal = (wyMV * vec4(wyNormal, 0)).xyz;

  gl_Position = wyMVP * vec4(wyPosition, 1);
}

