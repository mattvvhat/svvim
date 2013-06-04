
/**
 * Pass-through shader
 *
 */

varying vec4 color;
uniform float displacement;

void main() {

	gl_Position = ftransform();
  vec4 xyzt = gl_MultiTexCoord0;
  gl_TexCoord[0] = gl_MultiTexCoord0 + vec4(displacement, 0., 0., 0.);
  gl_TexCoord[1] = gl_MultiTexCoord0 - vec4(displacement, 0., 0., 0.);
}