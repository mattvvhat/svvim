uniform sampler2DRect maskTexture;
uniform sampler2DRect innerTexture;

uniform vec2 bounds;
uniform float alpha;

float gray (vec3 color) {
  return dot(color.rgb, vec3(.31, .5, .29));
}

void main() {
  vec2 pos = gl_TexCoord[0].st;
  vec4 mask = texture2DRect(maskTexture, pos);
  vec3 inner = texture2DRect(innerTexture, pos).rgb;
  vec3 finalColors = vec3(0);
  
  float baseg = gray(mask.rgb);
  float blendg = gray(inner.rgb);
  
  
  if (baseg > blendg) {
    finalColors = mask.rgb;
  }
  else {
    finalColors = inner.rgb;
  }
  
  gl_FragColor.rgb = finalColors;
  gl_FragColor.a = mask.a;
}