uniform sampler2DRect maskTexture;
uniform sampler2DRect innerTexture;
uniform vec3 fillColor;

float gray (vec3 color) {
  return dot(color.rgb, vec3(.31, .5, .29));
}

void main() {
  vec2 pos = gl_TexCoord[0].st;
  vec2 pos2 = gl_TexCoord[1].st;
  
  vec4 mask = texture2DRect(maskTexture, pos);
  vec4 mask2 = texture2DRect(maskTexture, pos2);
  
  vec3 inner = texture2DRect(innerTexture, gl_TexCoord[1].st).rgb;
  gl_FragColor.rgb = inner.rgb;
  gl_FragColor.a = 1. - gray((mask.rgb + mask2.rgb)/2.);
}