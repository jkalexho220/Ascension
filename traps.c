/* IT'S A TRAP */

/* spike trap */

/* arrow trap */

/* laser field trap */
rule laser_rooms_always
inactive
highFrequency
{
	int laser = 0;
	float scale = 0;
	if (yGetDatabaseCount("laserRooms") > 0) {
		yDatabaseNext("laserRooms");
		if (trTimeMS() > yGetVar("laserRooms", "next")) {
			if (yGetVar("laserRooms", "active") == 0) {
				ySetVar("laserRooms", "next", trTimeMS() + 3000);
				yVarToVector("laserRooms", "pos");
				for(x=yGetDatabaseCount("playerUnits"); >0) {
					if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
						removePlayerUnit();
					} else if (zDistanceToVectorSquared("playerUnits", "pos") < 400) {
						ySetVar("laserRooms", "active", 1);
						ySetVar("laserRooms", "next", trTimeMS());
						trSoundPlayFN("attackwarning.wav","1",-1,"","");
						trSoundPlayFN("automatonspcbirth.wav","1",-1,"","");
						break;
					}
				}
			} else {
				ySetVar("laserRooms", "next", yGetVar("laserRooms", "next") + 200);
				laser = yGetVar("laserRooms", "active");
				if (laser < 9) {
					laser = laser - 1;
					trUnitSelectClear();
					trUnitSelectByQV("laserRooms");
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("skypassagein.wav","1",-1,"","");
					}
					for(x=1;<=4) {
						trUnitSelectClear();
						trUnitSelect(""+(x+trQuestVarGet("laserRooms")+4*laser));
						trSetSelectedScale(3,0,21);
					}
				} else {
					laser = laser - 9;
					trUnitSelectClear();
					trUnitSelectByQV("laserRooms");
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("sky passage.wav","1",-1,"","");
						trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					}
					trQuestVarSet("next", trQuestVarGet("laserRooms") + 4*laser + 1);
					
					yAddToDatabase("shrinkingLasers", "next");
					yAddUpdateVar("shrinkingLasers", "timeout", trTimeMS() + 500);

					for(x=1;<=4) {
						trUnitSelectClear();
						trUnitSelect(""+(x+trQuestVarGet("laserRooms") + 4*laser));
						trUnitHighlight(3.0, false);
						trSetSelectedScale(6,0,21);
					}
					yVarToVector("laserRooms", "xLaser"+laser+"0");
					yVarToVector("laserRooms", "xLaser"+laser+"1");
					yVarToVector("laserRooms", "zLaser"+laser+"0");
					yVarToVector("laserRooms", "zLaser"+laser+"1");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							trVectorSetUnitPos("pos", "playerUnits");
							if (vectorInRectangle("pos", "xLaser"+laser+"0", "xLaser"+laser+"1")) {
								damagePlayerUnit(300);
							} else if (vectorInRectangle("pos", "zLaser"+laser+"0", "zLaser"+laser+"1")) {
								damagePlayerUnit(300);
							}
						}
					}
				}
				

				ySetVar("laserRooms", "active", yGetVar("laserRooms", "active") + 1);
				if (yGetVar("laserRooms", "active") == 9) {
					ySetVar("laserRooms", "next", yGetVar("laserRooms", "next") + 400);
				} else if (yGetVar("laserRooms", "active") == 17) {
					/* only reset if there are still units in the room */
					ySetVar("laserRooms", "active", 0);
					ySetVar("laserRooms", "next", yGetVar("laserRooms", "next") + 2000);
					yVarToVector("laserRooms", "pos");
					for(x=yGetDatabaseCount("playerUnits"); >0) {
						if (yDatabaseNext("playerUnits", true) == -1 || trUnitAlive() == false) {
							removePlayerUnit();
						} else if (zDistanceToVectorSquared("playerUnits", "pos") < 400) {
							ySetVar("laserRooms", "active", 1);
							break;
						}
					}
				}
			}
		}
	}
	if (yGetDatabaseCount("shrinkingLasers") > 0) {
		yDatabaseNext("shrinkingLasers");
		scale = yGetVar("shrinkingLasers", "timeout") - trTimeMS();
		if (scale > 0) {
			scale = 0.012 * scale;
		} else {
			scale = 0;
			yRemoveFromDatabase("shrinkingLasers");
		}
		for(x=0; <4) {
			trUnitSelectClear();
			trUnitSelect(""+(x+trQuestVarGet("shrinkingLasers")), true);
			trSetSelectedScale(scale, scale, 21);
		}
	}
}