/* IT'S A TRAP */

/* spike trap */

/* arrow trap */
int dLaserRooms = 0;
int xLaserRoomActive = 0;
int xLaserRoomNext = 0;
int xLaserRoomPos = 0;
int xLaserRoomXBottom = 0;
int xLaserRoomXTop = 0;
int xLaserRoomZBottom = 0;
int xLaserRoomZTop = 0;

int dShrinkingLasers = 0;
int xShrinkingLaserTimeout = 0;

void initializeLaserTrapDatabase() {
	dLaserRooms = xInitDatabase("laserRooms");
	xInitAddInt(dLaserRooms,"name");
	xLaserRoomActive = xInitAddInt(dLaserRooms,"active");
	xLaserRoomNext = xInitAddInt(dLaserRooms,"next");
	xLaserRoomPos = xInitAddVector(dLaserRooms,"pos");
	xLaserRoomXBottom = xLaserRoomPos + 1;
	xLaserRoomXTop = xLaserRoomXBottom + 8;
	xLaserRoomZBottom = xLaserRoomXTop + 8;
	xLaserRoomZTop = xLaserRoomZBottom + 8;
	for(x=32; > 0) {
		xInitAddVector(dLaserRooms,"laser"+x);
	}
	
	dShrinkingLasers = xInitDatabase("shrinkingLasers");
	xInitAddInt(dShrinkingLasers,"name");
	xShrinkingLaserTimeout = xInitAddInt(dShrinkingLasers,"timeout");
}

int CarouselRoomLast = 0;
int CarouselRoomLastSound = 0;

int dCarouselRooms = 0;
int xCarouselRoomAngle = 0;
int xCarouselRoomNext = 0;
int xCarouselRoomActive = 0;
int xCarouselRoomHitbox = 0;
int xCarouselRoomPos = 0;

void initializeCarouselTrapDatabase() {
	dCarouselRooms = xInitDatabase("carouselRooms");
	xInitAddInt(dCarouselRooms, "name");
	xCarouselRoomAngle = xInitAddFloat(dCarouselRooms, "angle");
	xCarouselRoomNext = xInitAddInt(dCarouselRooms, "next");
	xCarouselRoomActive = xInitAddBool(dCarouselRooms, "active", false);
	xCarouselRoomHitbox = xInitAddBool(dCarouselRooms, "hitbox", false);
	xCarouselRoomPos = xInitAddVector(dCarouselRooms, "pos");
}

/* laser field trap */
rule laser_rooms_always
inactive
highFrequency
{
	int laser = 0;
	float scale = 0;
	vector pos = vector(0,0,0);
	if (xGetDatabaseCount(dLaserRooms) > 0) {
		xDatabaseNext(dLaserRooms);
		if (trTimeMS() > xGetInt(dLaserRooms, xLaserRoomNext)) {
			if (xGetInt(dLaserRooms, xLaserRoomActive) == 0) {
				xSetInt(dLaserRooms, xLaserRoomNext, trTimeMS() + 3000);
				pos = xGetVector(dLaserRooms,xLaserRoomPos);
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits,xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (unitDistanceToVector(xGetInt(dPlayerUnits,xUnitName), pos) < 400) {
						xSetInt(dLaserRooms, xLaserRoomActive, 1);
						xSetInt(dLaserRooms, xLaserRoomNext, trTimeMS());
						trSoundPlayFN("attackwarning.wav","1",-1,"","");
						trSoundPlayFN("automatonspcbirth.wav","1",-1,"","");
						break;
					}
				}
			} else {
				xSetInt(dLaserRooms, xLaserRoomNext, xGetInt(dLaserRooms, xLaserRoomNext) + 200);
				laser = xGetInt(dLaserRooms, xLaserRoomActive);
				if (laser < 9) {
					laser = laser - 1;
					xUnitSelect(dLaserRooms,xUnitName);
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("skypassagein.wav","1",-1,"","");
					}
					for(x=1;<=4) {
						trUnitSelectClear();
						trUnitSelect(""+(x+xGetInt(dLaserRooms,xUnitName)+4*laser));
						trSetSelectedScale(3,0,21);
					}
				} else {
					laser = laser - 9;
					xUnitSelect(dLaserRooms,xUnitName);
					if (trUnitVisToPlayer()) {
						trSoundPlayFN("sky passage.wav","1",-1,"","");
						trSoundPlayFN("phoenixattack.wav","1",-1,"","");
					}
					xSetPointer(dShrinkingLasers,xAddDatabaseBlock(dShrinkingLasers));
					xSetInt(dShrinkingLasers,xUnitName,xGetInt(dLaserRooms,xUnitName) + 4*laser + 1);
					xSetInt(dShrinkingLasers,xShrinkingLaserTimeout, trTimeMS() + 500);
					
					for(x=1;<=4) {
						trUnitSelectClear();
						trUnitSelect(""+(x+xGetInt(dLaserRooms,xUnitName) + 4*laser));
						trUnitHighlight(3.0, false);
						trSetSelectedScale(6,0,21);
					}
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits,xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else {
							pos = kbGetBlockPosition(""+xGetInt(dPlayerUnits,xUnitName), true);
							if (vectorInRectangle(pos, xGetVector(dLaserRooms,xLaserRoomXBottom + laser),
									xGetVector(dLaserRooms,xLaserRoomXTop + laser))) {
								damagePlayerUnit(300);
							} else if (vectorInRectangle(pos, xGetVector(dLaserRooms,xLaserRoomZBottom + laser),
									xGetVector(dLaserRooms,xLaserRoomZTop + laser))) {
								damagePlayerUnit(300);
							}
						}
					}
					for(x=xGetDatabaseCount(dEnemies); >0) {
						xDatabaseNext(dEnemies);
						xUnitSelectByID(dEnemies,xUnitID);
						if (trUnitAlive() == false) {
							removeEnemy();
						} else {
							pos = kbGetBlockPosition(""+xGetInt(dEnemies,xUnitName));
							if (vectorInRectangle(pos, xGetVector(dLaserRooms,xLaserRoomXBottom + laser),
									xGetVector(dLaserRooms,xLaserRoomXTop + laser))) {
								trDamageUnit(300);
							} else if (vectorInRectangle(pos, xGetVector(dLaserRooms,xLaserRoomZBottom + laser),
									xGetVector(dLaserRooms,xLaserRoomZTop + laser))) {
								trDamageUnit(300);
							}
						}
					}
				}
				
				
				xSetInt(dLaserRooms, xLaserRoomActive, xGetInt(dLaserRooms, xLaserRoomActive) + 1);
				if (xGetInt(dLaserRooms, xLaserRoomActive) == 9) {
					xSetInt(dLaserRooms, xLaserRoomNext, xGetInt(dLaserRooms, xLaserRoomNext) + 400);
				} else if (xGetInt(dLaserRooms, xLaserRoomActive) == 17) {
					/* only reset if there are still units in the room */
					xSetInt(dLaserRooms, xLaserRoomActive, 0);
					xSetInt(dLaserRooms, xLaserRoomNext, xGetInt(dLaserRooms, xLaserRoomNext) + 2000);
					pos = xGetVector(dLaserRooms,xLaserRoomPos);
					for(x=xGetDatabaseCount(dPlayerUnits); >0) {
						xDatabaseNext(dPlayerUnits);
						xUnitSelectByID(dPlayerUnits,xUnitID);
						if (trUnitAlive() == false) {
							removePlayerUnit();
						} else if (unitDistanceToVector(xGetInt(dPlayerUnits, xUnitName), pos) < 400) {
							xSetInt(dLaserRooms, xLaserRoomActive, 1);
							break;
						}
					}
				}
			}
		}
	}
	if (xGetDatabaseCount(dShrinkingLasers) > 0) {
		xDatabaseNext(dShrinkingLasers);
		scale = xGetInt(dShrinkingLasers, xShrinkingLaserTimeout) - trTimeMS();
		if (scale > 0) {
			scale = 0.012 * scale;
		} else {
			scale = 0;
		}
		for(x=0; <4) {
			trUnitSelectClear();
			trUnitSelect(""+(x+xGetInt(dShrinkingLasers,xUnitName)), true);
			trSetSelectedScale(scale, scale, 21);
		}
		if (scale == 0) {
			xFreeDatabaseBlock(dShrinkingLasers);
		}
	}
}

rule carousel_rooms_always
inactive
highFrequency
{
	vector dir = vector(0,0,0);
	vector pos = vector(0,0,0);
	float angle = 0;
	float diff = trTimeMS() - CarouselRoomLast;
	CarouselRoomLast = trTimeMS();
	for (y=xGetDatabaseCount(dCarouselRooms); >0) {
		xDatabaseNext(dCarouselRooms);
		if (xGetBool(dCarouselRooms, xCarouselRoomActive)) {
			xUnitSelect(dCarouselRooms, xUnitName);
			if (trUnitVisToPlayer()) {
				if (trTime() > CarouselRoomLastSound) {
					trSoundPlayFN("inferno_loop.wav","1",-1,"","");
					CarouselRoomLastSound = trTime() + 5;
				}
			}
			angle = fModulo(6.283185, xGetFloat(dCarouselRooms, xCarouselRoomAngle) + 0.0004 * diff + 3.141592);
			xSetFloat(dCarouselRooms, xCarouselRoomAngle, angle);
			dir = xsVectorSet(xsCos(angle),0,xsSin(angle));
			trUnitSelectClear();
			trUnitSelect(""+(xGetInt(dCarouselRooms, xUnitName) + 1), true);
			trSetSelectedUpVector(4.0 * xsVectorGetX(dir),-1,4.0 * xsVectorGetZ(dir));
			dir = rotationMatrix(dir, 0, 1.0);
			trUnitSelectClear();
			trUnitSelect(""+(xGetInt(dCarouselRooms, xUnitName) + 2), true);
			trSetSelectedUpVector(4.0 * xsVectorGetX(dir),-1,4.0 * xsVectorGetZ(dir));
			
			if (trTimeMS() > xGetInt(dCarouselRooms, xCarouselRoomNext)) {
				xSetInt(dCarouselRooms, xCarouselRoomNext, xGetInt(dCarouselRooms, xCarouselRoomNext) + 200);
				xSetBool(dCarouselRooms, xCarouselRoomHitbox, xGetBool(dCarouselRooms, xCarouselRoomHitbox) == false);
				if (xGetBool(dCarouselRooms, xCarouselRoomHitbox)) {
					dir = rotationMatrix(dir, 0, 1.0);
				}
				pos = xGetVector(dCarouselRooms, xCarouselRoomPos) - dir * 16.0;
				for(x=xGetDatabaseCount(dPlayerUnits); >0) {
					xDatabaseNext(dPlayerUnits);
					xUnitSelectByID(dPlayerUnits, xUnitID);
					if (trUnitAlive() == false) {
						removePlayerUnit();
					} else if (rayCollision(dPlayerUnits, pos, dir, 32.0, 6.0)) {
						damagePlayerUnit(200.0);
					}
				}
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies, xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (rayCollision(dEnemies, pos, dir, 32.0, 6.0)) {
						trDamageUnit(200.0);
					}
				}
				xUnitSelect(dCarouselRooms, xUnitName);
				bool deactivate = true;
				xUnitSelect(dCarouselRooms, xUnitName);
				for(p=1; < ENEMY_PLAYER) {
					if (trUnitHasLOS(p)) {
						deactivate = false;
						break;
					}
				}
				if (deactivate) {
					xSetBool(dCarouselRooms, xCarouselRoomActive, false);
				}
			}
		} else {
			xUnitSelect(dCarouselRooms, xUnitName);
			for(p=1; < ENEMY_PLAYER) {
				if (trUnitHasLOS(p)) {
					xSetBool(dCarouselRooms, xCarouselRoomActive, true);
					xSetInt(dCarouselRooms, xCarouselRoomNext, trTimeMS());
					trUnitSelectClear();
					trUnitSelect(""+(xGetInt(dCarouselRooms, xUnitName) + 1), true);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trUnitOverrideAnimation(6,0,true,false,-1);
					trUnitSelectClear();
					trUnitSelect(""+(xGetInt(dCarouselRooms, xUnitName) + 2), true);
					trMutateSelected(kbGetProtoUnitID("Meteorite"));
					trUnitOverrideAnimation(6,0,true,false,-1);
					break;
				}
			}
		}
	}
}