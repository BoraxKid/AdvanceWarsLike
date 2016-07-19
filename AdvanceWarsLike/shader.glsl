uniform sampler2D texture;
uniform vec4 color;

void main()
{
	vec4 textureColor = texture2D(texture, vec2(gl_TexCoord[0]));
    float newColor = dot(textureColor.rgb, color.rgb);//vec3(0.299, 0.587, 0.114));
    gl_FragColor = vec4(newColor, newColor, newColor, textureColor.a) * color;
	//float gray = dot(textureColor.rgb, gl_Color);//vec3(0.299, 0.587, 0.114));
    //gl_FragColor = vec4(gray, gray, gray, textureColor.a);
}