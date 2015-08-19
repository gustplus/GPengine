#version 330
uniform int gDiv;
uniform mat4 gMVP;

void main()
{
	vec4 v0 = gl_in[0].gl_Position;
	vec4 v1 = gl_in[1].gl_Position;
	vec4 v2 = gl_in[2].gl_Position;

	float dx = abs(v0.x - v2.x) / gDiv;
	float dz = abs(v0.z - v1.z) / gDiv;

	float startX = v0.x;
	float startZ = v0.z;

	for(int row = gDiv; row >0 ; --row)
	{
		gl_Position = gMVP * vec4(startX, 0, startZ, 1);
		EmitVertex();
		for(int count = 0; count < 2 * row - 1; ++count)
		{
			if(count % 2 == 0)
			{
				gl_Position = gMVP * vec4(startX, 0, startZ + dz, 1);
			}else{
				gl_Position = gMVP * vec4(startX + dx, 0, startZ, 1);	
			}
			EmitVertex();
			startZ += dz;
		}
		startZ = v0.z;
		startX += dx;
		EndPrimitive();
	}
}