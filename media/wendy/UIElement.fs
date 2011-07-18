
uniform sampler2DRect image;

varying vec2 mapping;

void main()
{
  gl_FragColor = texture2DRect(image, mapping);
}

