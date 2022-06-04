rule gladiator_worlds_build_map
inactive
highFrequency
{
	float dist = 0;

	// black outer circle
	for(x=0; <= 145) {
		for(z=0; <= 145) {
			if (xsPow(x - 73, 2) + xsPow(z - 73, 2) > 1764) {
				trPaintTerrain(x, z, x, z, 2, 13, false);
			}
		}
	}
}