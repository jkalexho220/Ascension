void setupProtounitBounty(string proto = "", float armor = 0, int bounty = 2, float relicChance = 0, int relic = -1) {
	int p = kbGetProtoUnitID(proto);
	trQuestVarSet("proto"+p+"bounty", bounty);
	trQuestVarSet("proto"+p+"relicChance", relicChance);
	trQuestVarSet("proto"+p+"relic", relic);
	trQuestVarSet("proto"+p+"armor", armor);
	/* LOS */
	trModifyProtounit(proto, ENEMY_PLAYER, 2, 12);
	for(p=ENEMY_PLAYER; >0) {
		/* armor */
		trModifyProtounit(proto, p, 24, -1);
		trModifyProtounit(proto, p, 25, -1);
		trModifyProtounit(proto, p, 26, -1);
		trModifyProtounit(proto, p, 24, armor);
		trModifyProtounit(proto, p, 25, armor);
		trModifyProtounit(proto, p, 26, armor);
		/* damage bonus vs myth */
		trModifyProtounit(proto, p, 33, 9999999999999999999.0);
		trModifyProtounit(proto, p, 33, -9999999999999999999.0);
		trModifyProtounit(proto, p, 33, 1.0);
		trModifyProtounit(proto, p, 44, 9999999999999999999.0);
		trModifyProtounit(proto, p, 44, -9999999999999999999.0);
		trModifyProtounit(proto, p, 44, 1.0);
		/* damage bonus vs hero */
		trModifyProtounit(proto, p, 34, 9999999999999999999.0);
		trModifyProtounit(proto, p, 34, -9999999999999999999.0);
		trModifyProtounit(proto, p, 34, 1.0);
		trModifyProtounit(proto, p, 45, 9999999999999999999.0);
		trModifyProtounit(proto, p, 45, -9999999999999999999.0);
		trModifyProtounit(proto, p, 45, 1.0);
	}
	
}

bool checkEnemyDeactivated(int db = 0) {
	/* this only matters for enemies */
	if (xGetInt(db,xPlayerOwner) < ENEMY_PLAYER) {
		return(false);
	}
	int index = xGetInt(db,xSpecialIndex);
	if ((aiPlanGetUserVariableBool(dEnemies, xDirtyBit, index) == false) ||
		xGetInt(dEnemies, xUnitName, index) != xGetInt(db,xUnitName)) {
		return(true);
	}
	return(false);
}

void activateEnemy(int name = 0, int bounty = -1, int relic = -1) {
	int id = kbGetBlockID(""+name);
	int proto = kbGetUnitBaseTypeID(id);
	if (bounty < 0) {
		bounty = trQuestVarGet("proto"+proto+"bounty");
	}
	if (relic < 0) {
		trQuestVarSetFromRand("relicChance", 0, 1, false);
		if (trQuestVarGet("relicChance") < trQuestVarGet("proto"+proto+"relicChance")) {
			relic = trQuestVarGet("proto"+proto+"relic");
			if (relic == -1) {
				relic = trQuestVarGet("stage");
			}
		}
	}
	float armor = trQuestVarGet("proto"+proto+"armor");
	vector pos = kbGetBlockPosition(""+name);
	xSetPointer(dEnemies, xAddDatabaseBlock(dEnemies));
	xSetInt(dEnemies, xUnitName, name);
	xSetInt(dEnemies, xPlayerOwner, ENEMY_PLAYER);
	xSetInt(dEnemies, xUnitID, id);
	trQuestVarSetFromRand("bounty", bounty / 2, bounty, true);
	xSetInt(dEnemies, xBounty, 1*trQuestVarGet("bounty"));
	xSetInt(dEnemies, xDropRelic, relic);
	xSetVector(dEnemies,xUnitPos,pos);
	xSetFloat(dEnemies,xMagicResist,armor);
	xSetFloat(dEnemies,xPhysicalResist,armor);
	for(p=1; < ENEMY_PLAYER) {
		if (trQuestVarGet("p"+p+"rideLightning") == 1) {
			int db = trQuestVarGet("p"+p+"rideLightningTargets");
			xSetPointer(db, xAddDatabaseBlock(db));
			xSetInt(db,xUnitName,name);
			xSetInt(db,xDatabaseIndex,xGetNewestPointer(dEnemies));
		}
	}
	activateSpecialUnit(name, dEnemies, proto, ENEMY_PLAYER);
}


rule setup_enemies
inactive
highFrequency
{
	trModifyProtounit("Minion", ENEMY_PLAYER, 8, -99);
	trModifyProtounit("Minion", 1, 8, -99);
	
	setupProtounitBounty("Nemean Lion", 0, 0, 0);
	setupProtounitBounty("King Folstag", 0, 0, 0);
	setupProtounitBounty("Chimera", 0.25, 0, 0);
	setupProtounitBounty("Shade of Hades", 0.25, 0, 0);
	setupProtounitBounty("Helepolis", 0.25, 0, 0);
	setupProtounitBounty("Scylla", 0.5, 0, 0);
	setupProtounitBounty("Guardian", 0.5, 0, 0);
	setupProtounitBounty("Pharaoh of Osiris XP", 0.47, 0, 0);
	
	setupProtounitBounty("Golden Lion", 0.3, 3);
	setupProtounitBounty("Anubite", 0.3, 4, 0.03);
	setupProtounitBounty("Terracotta Soldier", 0.3, 4, 0.03);
	setupProtounitBounty("Sphinx", 0.3, 5, 0.05);
	setupProtounitBounty("Petsuchos", 0.1, 6, 0.1, RELIC_ATTACK_RANGE);
	
	setupProtounitBounty("Walking Woods Marsh", 0.2, 3);
	setupProtounitBounty("Dryad", 0.2, 4, 0.03);
	setupProtounitBounty("Centaur", 0.1, 4, 0.03);
	setupProtounitBounty("Medusa", 0.2, 8, 0.15, RELIC_SPELL_DURATION);
	setupProtounitBounty("Mountain Giant", 0.3, 10, 0.1);
	
	setupProtounitBounty("Fenris Wolf", 0.2, 4, 0.03);
	setupProtounitBounty("Valkyrie", 0, 5, 0.05);
	setupProtounitBounty("Ballista", 0.1, 4, 0.03);
	setupProtounitBounty("Frost Giant", 0.3, 10, 0.1);
	
	setupProtounitBounty("Wadjet", 0.2, 4, 0.03);
	setupProtounitBounty("Scorpion Man", 0.3, 6, 0.05);
	setupProtounitBounty("Scarab", 0.9, 10, 0.08);
	setupProtounitBounty("Mummy", 0.6, 12, 0.1);
	
	setupProtounitBounty("Cyclops", 0.2, 6, 0.05);
	setupProtounitBounty("Satyr", 0.1, 4, 0.03);
	setupProtounitBounty("Behemoth", 0.9, 10, 0.08);
	setupProtounitBounty("Avenger", 0.2, 12, 0.1);
	
	setupProtounitBounty("Automaton SPC", 0.4, 4, 0);
	setupProtounitBounty("Colossus", 0.2, 10, 0.1);
	setupProtounitBounty("Battle Boar", 0.4, 8, 0.08);
	setupProtounitBounty("Fire Siphon", 0.1, 8, 0.1);
	
	setupProtounitBounty("Servant", 0.5, 5, 0.03);
	setupProtounitBounty("Nereid", 0.3, 7, 0.05);
	setupProtounitBounty("Kraken", 0.4, 9, 0.08);
	setupProtounitBounty("Hydra", 0.4, 10, 0.1);
	
	setupProtounitBounty("Griffon", 0.6, 6, 0.03);
	setupProtounitBounty("Einheriar", 0.6, 7, 0.03);
	setupProtounitBounty("Statue of Lightning", 0.5, 8, 0.05);
	setupProtounitBounty("Lampades", 0.8, 12, 0.1);
	
	setupProtounitBounty("Shade XP", 0, 0, 0);
	trModifyProtounit("Shade XP", 0, 1, -1.8);
	trModifyProtounit("Shade XP", ENEMY_PLAYER, 1, -1.8);
	
	setupProtounitBounty("Hero Boar", 0, 0, 0);
	setupProtounitBounty("Hero Boar 2", 0, 0, 0);
	trModifyProtounit("Hero Boar", ENEMY_PLAYER, 27, -999); // attack
	trModifyProtounit("Hero Boar 2", ENEMY_PLAYER, 27, -999); // attack
	
	for(class = 1; <= 16) {
		setupProtounitBounty(kbGetProtoUnitName(xGetInt(dClass,xClassProto,class)),
			trQuestVarGet("proto"+xGetInt(dClass,xClassProto,class)+"armor"), 8, 0);
	}
	
	/* ballista projectiles */
	for(p=ENEMY_PLAYER; >0) {
		trModifyProtounit("Ballista", p, 13, -3);
		trModifyProtounit("Ballista", p, 11, -12);
	}
	trModifyProtounit("Ballista Shot", ENEMY_PLAYER, 1, -19);
	trModifyProtounit("Priest Projectile", ENEMY_PLAYER, 1, -20);
	trModifyProtounit("Hero Greek Achilles", ENEMY_PLAYER, 5, 99);
	
	
	trModifyProtounit("Outpost", ENEMY_PLAYER, 0, 9999999999999999999.0);
	trModifyProtounit("Outpost", ENEMY_PLAYER, 0, -9999999999999999999.0);
	trModifyProtounit("Outpost", ENEMY_PLAYER, 0, 47);
	trModifyProtounit("Outpost", ENEMY_PLAYER, 24, 1);
	trModifyProtounit("Outpost", ENEMY_PLAYER, 25, 1);
	trModifyProtounit("Outpost", ENEMY_PLAYER, 26, 1);
	
	trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 0, 9999999999999999999.0);
	trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 0, -9999999999999999999.0);
	trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 0, 47000);
	trModifyProtounit("Pharaoh of Osiris XP", ENEMY_PLAYER, 2, 20);
	
	trModifyProtounit("Stymphalian Bird", 0, 1, 2);
	
	xsDisableSelf();
}

void ballistaShotPop(vector pos = vector(0,0,0)) {
	int p = xGetInt(dBallistaShots,xPlayerOwner);
	int db = opponentDatabaseName(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		xUnitSelectByID(db,xUnitID);
		if (trUnitAlive() == false) {
			removeOpponentUnit(p);
		} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 16) {
			damageOpponentUnit(p, 200);
		}
	}
	
	xUnitSelect(dBallistaShots,xBallistaShot1);
	trUnitDestroy();
	xUnitSelect(dBallistaShots,xBallistaShot2);
	trUnitDestroy();
	xUnitSelect(dBallistaShots,xUnitName);
	trDamageUnitPercent(100);
	trUnitChangeProtoUnit("Meteorite");
	trSoundPlayFN("fireball launch.wav","1",-1,"","");
	xFreeDatabaseBlock(dBallistaShots);
}

bool spawnLightning(vector pos = vector(0,0,0), int p = 0) {
	vector loc = vectorToGrid(pos);
	if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) == false) {
		trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trArmySelect("0,0");
		trUnitChangeProtoUnit("Arkantos Boost SFX");
		int next = trGetNextUnitScenarioNameNumber();
		trArmyDispatch(""+ENEMY_PLAYER+",0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
		trUnitSelectClear();
		trUnitSelect(""+next,true);
		trUnitSetStance("Passive");
		trMutateSelected(kbGetProtoUnitID("Lampades Bolt"));
		xSetPointer(dYeebLightning, xAddDatabaseBlock(dYeebLightning));
		xSetInt(dYeebLightning,xUnitName,next);
		xSetInt(dYeebLightning,xPlayerOwner,p);
		xSetInt(dYeebLightning,xTimeout, trTimeMS() + 2000);
		return(true);
	}
	return(false);
}

int findShooter(int db = 0, vector pos = vector(0,0,0)) {
	int closest = 0;
	int id = 0;
	float dist = 0;
	float closestDistance = 25;
	for (x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		id = xGetInt(db,xUnitID);
		xUnitSelectByID(db,xUnitID);
		if (trUnitAlive() == false) {
			xFreeDatabaseBlock(db);
		} else if (kbUnitGetAnimationActionType(id) == 12) {
			trUnitSelectClear();
			trUnitSelect(""+nextproj,true);
			if (trUnitIsOwnedBy(xGetInt(db,xPlayerOwner))) {
				dist = unitDistanceToVector(xGetInt(db,xUnitName),pos);
				if (dist < closestDistance) {
					closestDistance = dist;
					closest = xGetPointer(db);
				}
			}
		}
	}
	return(closest);
}

void specialUnitsAlways() {
	xsSetContextPlayer(0);
	int p = 0;
	int proto = 0;
	int id = 0;
	int db = 0;
	int target = 0;
	int action = 0;
	int closest = 0;
	float amt = 0;
	float dist = 0;
	float angle = 0;
	bool hit = false;
	vector start = vector(0,0,0);
	vector end = vector(0,0,0);
	vector dir = vector(0,0,0);
	vector pos = vector(0,0,0);
	vector loc = vector(0,0,0);
	
	for (x=xsMin(5, xGetDatabaseCount(dBallistaShots)); >0) {
		xDatabaseNext(dBallistaShots);
		xUnitSelect(dBallistaShots,xUnitName);
		pos = kbGetBlockPosition(""+xGetInt(dBallistaShots,xUnitName));
		
		loc = vectorToGrid(pos);
		if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) || trTimeMS() > xGetInt(dBallistaShots, xTimeout)) {
			ballistaShotPop(pos);
		} else if ((xGetInt(dBallistaShots, xPlayerOwner) == ENEMY_PLAYER) || PvP) {
			action = 0;
			for(p=1; < ENEMY_PLAYER) {
				if (PvP && (p == xGetInt(dBallistaShots, xPlayerOwner))) {
					continue;
				}
				action = action + trCountUnitsInArea(""+xGetInt(dBallistaShots,xUnitName),p,"Unit",2);
			}
			if (action > 0) {
				ballistaShotPop(pos);
			}
		} else if (trCountUnitsInArea(""+xGetInt(dBallistaShots,xUnitName),ENEMY_PLAYER,"Unit",2) > 0) {
			ballistaShotPop(pos);
		}
	}
	
	trQuestVarSet("sound", 0);
	for(x=xsMin(10, xGetDatabaseCount(dDelayLasers)); >0) {
		xDatabaseNext(dDelayLasers);
		xUnitSelect(dDelayLasers,xUnitName);
		if (xGetInt(dDelayLasers, xLaserPhase) == 0) {
			if (trTimeMS() > xGetInt(dDelayLasers, xLaserNext)) {
				xSetInt(dDelayLasers, xLaserPhase, 1);
				xSetInt(dDelayLasers, xLaserNext, trTimeMS() + 500);
				trSetSelectedScale(6.0,6.0,xGetFloat(dDelayLasers,xLaserDist) * 1.3);
				trUnitHighlight(3.0,false);
				trQuestVarSet("sound", 1);
				start = kbGetBlockPosition(""+xGetInt(dDelayLasers,xUnitName));
				p = xGetInt(dDelayLasers,xPlayerOwner);
				dir = xGetVector(dDelayLasers,xLaserDir);
				db = opponentDatabaseName(p);
				for(y=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db,xUnitID);
					if (trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (rayCollision(db,start,dir,xGetFloat(dDelayLasers,xLaserDist),1.0)) {
						damageOpponentUnit(p, 300);
					}
				}
			}
		} else {
			amt = 0.012 * (xGetInt(dDelayLasers, xLaserNext) - trTimeMS());
			if (amt < 0) {
				trUnitDestroy();
				xFreeDatabaseBlock(dDelayLasers);
			} else {
				trSetSelectedScale(amt, amt, xGetFloat(dDelayLasers, xLaserDist) * 1.3);
			}
		}
	}
	
	if (trQuestVarGet("sound") == 1) {
		trSoundPlayFN("sky passage.wav", "1", -1,"","");
		trSoundPlayFN("phoenixattack.wav", "1", -1,"","");
		trQuestVarSet("sound", 0);
	}
	
	/*
	Projectiles from attacks
	*/
	while(nextproj < trGetNextUnitScenarioNameNumber()) {
		trUnitSelectClear();
		trUnitSelect(""+nextproj,true);
		if (trUnitIsOwnedBy(0) == false) {
			id = kbGetBlockID(""+nextproj, true);
			proto = kbGetUnitBaseTypeID(id);
			if (proto == kbGetProtoUnitID("Ballista Shot")) {
				pos = kbGetBlockPosition(""+nextproj,true);
				closest = findShooter(dBallistas,pos);
				if (closest > 0) {
					xSetPointer(dBallistas, closest);
					id = xGetInt(dBallistas, xUnitID);
					xsSetContextPlayer(xGetInt(dBallistas,xPlayerOwner));
					target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
					xsSetContextPlayer(0);
					
					start = kbGetBlockPosition(""+xGetInt(dBallistas,xUnitName),true);
					end = kbGetBlockPosition(""+target,true);
					
					pos = vectorSetAsTargetVector(start, end, 100.0);
					
					trUnitSelectClear();
					trUnitSelect(""+nextproj, true);
					trUnitConvert(ENEMY_PLAYER);
					trUnitChangeProtoUnit("Transport Ship Greek");
					
					xSetPointer(dBallistaShots, xAddDatabaseBlock(dBallistaShots));
					xSetInt(dBallistaShots, xBallistaShot1, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0", "Dwarf", 1, 1,0,1,0,true);
					xSetInt(dBallistaShots, xUnitName, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0", "Dwarf", 1, 1,0,1,0,false);
					trArmySelect("1,0");
					trUnitConvert(ENEMY_PLAYER);
					trImmediateUnitGarrison(""+nextproj);
					trUnitChangeProtoUnit("Dwarf");
					
					xSetInt(dBallistaShots, xBallistaShot2, nextproj);
					xSetInt(dBallistaShots, xTimeout, trTimeMS() + 5000);
					xSetInt(dBallistaShots, xPlayerOwner, xGetInt(dBallistas, xPlayerOwner));
					
					trUnitSelectClear();
					trUnitSelect(""+xGetInt(dBallistaShots, xBallistaShot1), true);
					trUnitSelect(""+1*nextproj, true);
					trUnitChangeProtoUnit("Relic");
					
					xUnitSelect(dBallistaShots,xUnitName);
					trMutateSelected(kbGetProtoUnitID("Hero Greek Achilles"));
					
					xUnitSelect(dBallistaShots,xBallistaShot1);
					trImmediateUnitGarrison(""+xGetInt(dBallistaShots,xUnitName));
					
					trMutateSelected(kbGetProtoUnitID("Ballista Shot"));
					trUnitSelectClear();
					trUnitSelect(""+nextproj, true);
					trImmediateUnitGarrison(""+xGetInt(dBallistaShots,xUnitName));
					trMutateSelected(kbGetProtoUnitID("Fire Siphon Fire"));
					trUnitConvert(0);
					xUnitSelect(dBallistaShots, xUnitName);
					trMutateSelected(kbGetProtoUnitID("Priest Projectile"));
					trSetSelectedScale(0,50,0);
					trSetUnitOrientation(getUnitVector(start, end), xsVectorSet(0,1,0), true);
					trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos), -1, false);
					xUnitSelect(dBallistaShots,xBallistaShot1);
					trUnitConvert(0);
				}
			} else if (proto == kbGetProtoUnitID("Fire Siphon Fire")) {
				if (xGetDatabaseCount(dFireSiphons) > 0) {
					pos = kbGetBlockPosition(""+nextproj,true);
					trUnitChangeProtoUnit("Lightning sparks");
					closest = findShooter(dFireSiphons,pos);
					if (closest > 0) {
						xSetPointer(dFireSiphons,closest);
						id = xGetInt(dFireSiphons,xUnitID);
						xsSetContextPlayer(xGetInt(dFireSiphons,xPlayerOwner));
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						
						trSoundPlayFN("skypassagein.wav","1",-1,"","");
						start = kbGetBlockPosition(""+xGetInt(dFireSiphons,xUnitName),true);
						pos = kbGetBlockPosition(""+target,true);
						dir = getUnitVector(start, pos);
						shootLaser(start, dir, 60.0, xGetInt(dFireSiphons,xPlayerOwner));
					}
				}
			} else if (proto == kbGetProtoUnitID("Mummy Flies")) {
				if (xGetDatabaseCount(dMummies) > 0) {
					pos = kbGetBlockPosition(""+nextproj,true);
					trUnitChangeProtoUnit("Rocket");
					closest = findShooter(dMummies,pos);
					if (closest > 0) {
						xSetPointer(dMummies, closest);
						id = xGetInt(dMummies,xUnitID);
						xsSetContextPlayer(xGetInt(dMummies,xPlayerOwner));
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						
						start = kbGetBlockPosition(""+xGetInt(dMummies,xUnitName),true);
						pos = kbGetBlockPosition(""+target,true);
						dir = getUnitVector(start, pos);
						
						addGenericProj(dMummyBalls,start,dir,xGetInt(dMummies,xPlayerOwner));
						xSetVector(dMummyBalls,xProjPrev, start);
						xSetFloat(dMummyBalls,xProjDist,4.0);
						xSetInt(dMummyBalls,xProjType, STATUS_SILENCE);
					}
				}
			}
		}
		nextproj = 1 + nextproj;
	}
	
	if (xGetDatabaseCount(dSphinxes) > 0) {
		xDatabaseNext(dSphinxes);
		id = xGetInt(dSphinxes,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dSphinxes,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dSphinxes)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Sphinx");
			}
			xFreeDatabaseBlock(dSphinxes);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dSphinxes,xSpecialIndex)) == 1) {
			xSetInt(dSphinxes,xSpecialStep,2);
		} else if (trTimeMS() > xGetInt(dSphinxes,xSpecialNext)) {
			switch(xGetInt(dSphinxes, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xSetInt(dSphinxes, xSpecialNext, trTimeMS() + 1600);
						xSetInt(dSphinxes, xSpecialStep, 1);
						trUnitOverrideAnimation(39,0,false,false,-1);
						pos = kbGetBlockPosition(""+xGetInt(dSphinxes,xUnitName));
						db = opponentDatabaseName(p);
						for(x=xGetDatabaseCount(db); >0) {
							xDatabaseNext(db);
							xUnitSelectByID(db,xUnitID);
							if (trUnitAlive() == false) {
								removeOpponentUnit(p);
							} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 16) {
								silenceUnit(db,5,p);
							}
						}
						if (p == ENEMY_PLAYER) {
							if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 1) {
								if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,trCurrentPlayer()), pos) < 16) {
									trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 2);
									xsEnableRule("nick_next_dialog");
								}
							}
						}
					}
				}
				case 1:
				{
					xSetInt(dSphinxes, xSpecialNext, trTimeMS() + 10000);
					xSetInt(dSphinxes, xSpecialStep, 0);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		}
	}
	
	if (xGetDatabaseCount(dMountainGiants) > 0) {
		xDatabaseNext(dMountainGiants);
		id = xGetInt(dMountainGiants,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dMountainGiants,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false) {
			trQuestVarSet("giantKills", 1 + trQuestVarGet("giantKills"));
			trUnitChangeProtoUnit("Mountain Giant");
			xFreeDatabaseBlock(dMountainGiants);
		} else if (checkEnemyDeactivated(dMountainGiants)) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock(dMountainGiants);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dMountainGiants, xSpecialIndex)) == 1) {
			xSetInt(dMountainGiants, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dMountainGiants, xSpecialNext)) {
			switch(xGetInt(dMountainGiants, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetVector(dMountainGiants,xSpecialTarget,kbGetBlockPosition(""+target));
						
						xSetInt(dMountainGiants, xSpecialNext, trTimeMS() + 1800);
						xSetInt(dMountainGiants, xSpecialStep, 1);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					end = xGetVector(dMountainGiants,xSpecialTarget);
					db = opponentDatabaseName(p);
					for(x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (unitDistanceToVector(xGetInt(db,xUnitName), end) < 4) {
							damageOpponentUnit(p, 100 + 100 * trQuestVarGet("stage"));
							if (xGetBool(db, xIsHero) && trCurrentPlayer() == xGetInt(db, xPlayerOwner)) {
								trCameraShake(0.7, 0.7);
							}
						}
					}
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(end),0,xsVectorGetZ(end),45,true);
					trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(end),0,xsVectorGetZ(end),135,false);
					trArmySelect("1,0");
					trUnitChangeProtoUnit("Tartarian Gate Flame");
					xSetInt(dMountainGiants, xSpecialStep, 2);
					xSetInt(dMountainGiants, xSpecialNext, xGetInt(dMountainGiants, xSpecialNext) + 1200);
					
				}
				case 2:
				{
					xSetInt(dMountainGiants, xSpecialStep, 0);
					xSetInt(dMountainGiants, xSpecialNext, trTimeMS() + 15000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dMountainGiants, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dMountainGiants, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dMountainGiants, xSpecialStep) == 1) {
				xSetInt(dMountainGiants, xSpecialStep, 0);
				xSetInt(dMountainGiants, xSpecialNext, trTimeMS() + 15000);
			}
		}
	}
	
	for(x=xsMin(4, xGetDatabaseCount(dMedusaBalls)); >0) {
		action = processGenericProj(dMedusaBalls);
		p = xGetInt(dMedusaBalls, xPlayerOwner);
		db = opponentDatabaseName(p);
		pos = kbGetBlockPosition(""+xGetInt(dMedusaBalls,xUnitName));
		loc = vectorToGrid(pos);
		for(y=xGetDatabaseCount(db); >0) {
			xDatabaseNext(db);
			xUnitSelectByID(db,xUnitID);
			if (trUnitAlive() == false) {
				removeOpponentUnit(p);
			} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 2.0) {
				stunUnit(db, 3.0, p);
				action = PROJ_REMOVE;
				break;
			}
		}
		if (action == PROJ_FALLING) {
			xUnitSelect(dMedusaBalls,xMedusaBallTarget);
			if (trUnitAlive()) {
				end = kbGetBlockPosition(""+xGetInt(dMedusaBalls,xMedusaBallTarget));
				dir = getUnitVector(pos,end);
				xSetVector(dMedusaBalls,xProjDir,dir);
			}
		} else if (action == PROJ_BOUNCE) {
			xSetInt(dMedusaBalls, xMedusaBallBounces, xGetInt(dMedusaBalls, xMedusaBallBounces) - 1);
		} else if (action == PROJ_REMOVE ||
			terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) ||
			xGetInt(dMedusaBalls, xMedusaBallBounces) <= 0) {
			xUnitSelectByID(dMedusaBalls,xUnitID);
			trUnitChangeProtoUnit("Conversion SFX");
			xFreeDatabaseBlock(dMedusaBalls);
		}
	}
	
	if (xGetDatabaseCount(dMedusas) >0) {
		xDatabaseNext(dMedusas);
		id = xGetInt(dMedusas,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dMedusas, xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dMedusas)) {
			trUnitChangeProtoUnit("Medusa");
			xFreeDatabaseBlock(dMedusas);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dMedusas, xSpecialIndex)) == 1) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xSetInt(dMedusas, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dMedusas, xSpecialNext)) {
			switch(xGetInt(dMedusas, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 12) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetInt(dMedusas, xSpecialTarget, target);
						
						xSetInt(dMedusas, xSpecialStep, 1);
						xSetInt(dMedusas, xSpecialNext, trTimeMS() + 1200);
						trUnitOverrideAnimation(40,0,false,false,-1);
					}
				}
				case 1:
				{
					end = kbGetBlockPosition(""+xGetInt(dMedusas, xSpecialTarget));
					start = kbGetBlockPosition(""+xGetInt(dMedusas,xUnitName));
					dir = getUnitVector(start, end);
					addGenericProj(dMedusaBalls,start,dir,p);
					xSetInt(dMedusaBalls, xMedusaBallTarget, xGetInt(dMedusas, xSpecialTarget));
					xSetInt(dMedusaBalls, xMedusaBallBounces, 10);
					xSetInt(dMedusas, xSpecialStep, 2);
					xSetInt(dMedusas, xSpecialNext, xGetInt(dMedusas, xSpecialNext) + 800);
				}
				case 2:
				{
					xSetInt(dMedusas, xSpecialStep, 0);
					xSetInt(dMedusas, xSpecialNext, trTimeMS() + 18000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dMedusas, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dMedusas, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dMedusas, xSpecialStep) == 1) {
				xSetInt(dMedusas, xSpecialStep, 0);
				xSetInt(dMedusas, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	/*
	xSpecialStep = valkyrie silenced status
	xSpecialNext = valkyrie sfx
	*/
	if (xGetDatabaseCount(dValkyries) > 0) {
		xDatabaseNext(dValkyries);
		id = xGetInt(dValkyries,xUnitID);
		p = xGetInt(dValkyries,xPlayerOwner);
		db = databaseName(p);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false || checkEnemyDeactivated(dValkyries)) {
			xUnitSelect(dValkyries,xSpecialNext);
			trUnitDestroy();
			xFreeDatabaseBlock(dValkyries);
		} else {
			id = xGetPointer(db);
			if (xSetPointer(db, xGetInt(dValkyries, xSpecialIndex))) {
				xSetFloat(db, xMagicResist, 1.0 - xGetInt(db, xSilenceStatus));
				if (xGetInt(db, xSilenceStatus) != xGetInt(dValkyries, xSpecialStep)) {
					xSetInt(dValkyries, xSpecialStep, xGetInt(db, xSilenceStatus));
					xUnitSelect(dValkyries,xSpecialNext);
					if (xGetInt(db, xSilenceStatus) == 1) {
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					} else {
						trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					}
				}
				xSetPointer(db, id);
			}
		}
	}
	
	if (xGetDatabaseCount(dDryads) > 0) {
		xDatabaseNext(dDryads);
		id = xGetInt(dDryads,xUnitID);
		p = xGetInt(dDryads, xPlayerOwner);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			if (xGetInt(dDryads, xSpecialStep) == 0) {
				db = opponentDatabaseName(p);
				pos = xGetVector(dDryads,xSpecialNext);
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Lampades Blood");
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					id = xGetInt(db,xUnitID);
					trUnitSelectClear();
					trUnitSelectByID(id);
					if (trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 16) {
						poisonUnit(db, 10.0, 5.0 * trQuestVarGet("stage"), p);
					}
				}
				if (p == ENEMY_PLAYER) {
					if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 2) {
						if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,trCurrentPlayer()), pos) < 16) {
							trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 3);
							xsEnableRule("nick_next_dialog");
						}
					}
				}
			}
			xFreeDatabaseBlock(dDryads);
		} else if (checkEnemyDeactivated(dDryads)) {
			xFreeDatabaseBlock(dDryads);
		} else {
			db = databaseName(p);
			xSetVector(dDryads, xSpecialNext, kbGetBlockPosition(""+xGetInt(dDryads,xUnitName)));
			xSetInt(dDryads, xSpecialStep, xGetInt(db, xSilenceStatus, xGetInt(dDryads, xSpecialIndex)));
		}
	}
	
	if (xGetDatabaseCount(dScarabs) > 0) {
		xDatabaseNext(dScarabs);
		id = xGetInt(dScarabs,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false || checkEnemyDeactivated(dScarabs)) {
			if (xGetInt(dScarabs, xSpecialStep) == 0) {
				pos = xGetVector(dScarabs,xSpecialNext);
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Pestilence SFX1");
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Victory Marker");
			}
			xFreeDatabaseBlock(dScarabs);
		} else if (checkEnemyDeactivated(dScarabs)) {
			xFreeDatabaseBlock(dScarabs);
		} else {
			db = databaseName(xGetInt(dScarabs,xPlayerOwner));
			xSetVector(dScarabs,xSpecialNext,kbGetBlockPosition(""+xGetInt(dScarabs,xUnitName)));
			xSetInt(dScarabs, xSpecialStep, xGetInt(db, xSilenceStatus, xGetInt(dScarabs, xSpecialIndex)));
		}
	}
	
	if (xGetDatabaseCount(dAutomatonBombs) > 0) {
		xDatabaseNext(dAutomatonBombs);
		if (trTimeMS() > xGetInt(dAutomatonBombs, xTimeout)) {
			p = xGetInt(dAutomatonBombs,xPlayerOwner);
			db = opponentDatabaseName(p);
			pos = kbGetBlockPosition(""+xGetInt(dAutomatonBombs,xUnitName));
			xUnitSelect(dAutomatonBombs,xUnitName);
			trDamageUnitPercent(-100);
			trUnitChangeProtoUnit("Tartarian Gate Flame");
			xUnitSelect(dAutomatonBombs,xUnitName);
			trDamageUnitPercent(-100);
			trSetSelectedScale(1.0,1.0,0.4);
			xFreeDatabaseBlock(dAutomatonBombs);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				id = xGetInt(db,xUnitID);
				xUnitSelectByID(db,xUnitID);
				if (trUnitAlive() == false) {
					removePlayerUnit();
				} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 16) {
					damageOpponentUnit(p, 200);
				}
			}
		}
	}
	
	if (xGetDatabaseCount(dAutomatons) > 0) {
		xDatabaseNext(dAutomatons);
		id = xGetInt(dAutomatons,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false) {
			if (xGetInt(dAutomatons, xSpecialStep) == 0) {
				trUnitHighlight(3.0, true);
				trSoundPlayFN("gatherpoint.wav","1",-1,"","");
				xSetPointer(dAutomatonBombs, xAddDatabaseBlock(dAutomatonBombs));
				xSetInt(dAutomatonBombs,xUnitName,xGetInt(dAutomatons,xUnitName));
				xSetInt(dAutomatonBombs,xPlayerOwner,xGetInt(dAutomatons,xPlayerOwner));
				xSetInt(dAutomatonBombs,xTimeout,trTimeMS() + 3000);
			}
			xFreeDatabaseBlock(dAutomatons);
		} else if (checkEnemyDeactivated(dAutomatons)) {
			xFreeDatabaseBlock(dAutomatons);
		} else {
			db = databaseName(xGetInt(dAutomatons,xPlayerOwner));
			xSetInt(dAutomatons, xSpecialStep, xGetInt(db, xSilenceStatus, xGetInt(dAutomatons, xSpecialIndex)));
		}
	}
	
	if(xGetDatabaseCount(dFrostGiants) >0) {
		xDatabaseNext(dFrostGiants);
		id = xGetInt(dFrostGiants,xUnitID);
		p = xGetInt(dFrostGiants,xPlayerOwner);
		trUnitSelectClear();
		trUnitSelectByID(id);
		db = databaseName(p);
		if (trUnitAlive() == false) {
			trQuestVarSet("giantKills", 1 + trQuestVarGet("giantKills"));
			if (trQuestVarGet("frostGiantHunt") == 1) {
				trQuestVarSet("frostGiantBones", 1 + trQuestVarGet("frostGiantBones"));
			}
			trUnitChangeProtoUnit("Frost Giant");
			xFreeDatabaseBlock(dFrostGiants);
		} else if (checkEnemyDeactivated(dFrostGiants)) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock(dFrostGiants);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dFrostGiants, xSpecialIndex)) == 1) {
			xSetInt(dFrostGiants, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dFrostGiants, xSpecialNext)) {
			switch(xGetInt(dFrostGiants, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetInt(dFrostGiants, xSpecialTarget, target);
						xSetInt(dFrostGiants, xSpecialStep, 1);
						xSetInt(dFrostGiants, xSpecialNext, trTimeMS() + 1400);
						trUnitOverrideAnimation(40,0,false,false,-1);
					}
				}
				case 1:
				{
					action = 0;
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						if (xGetInt(dFrostGiants, xSpecialTarget) == xGetInt(db,xUnitName)) {
							xUnitSelectByID(db,xUnitID);
							stunUnit(db, 3.0, p);
							action = 1;
							break;
						}
					}
					xSetInt(dFrostGiants, xSpecialStep, 2);
					xSetInt(dFrostGiants, xSpecialNext, xGetInt(dFrostGiants, xSpecialNext) + 600);
					if (action == 0) {
						xSetInt(dFrostGiants, xSpecialTarget, -1);
					} else if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 3) {
						if (xGetInt(dFrostGiants, xSpecialTarget) == xGetInt(dPlayerData,xPlayerUnit,trCurrentPlayer())) {
							trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 4);
							xsEnableRule("nick_next_dialog");
						}
					}
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt(dFrostGiants, xSpecialStep, 0);
					if (xGetInt(dFrostGiants, xSpecialTarget) == -1) {
						xSetInt(dFrostGiants, xSpecialNext, trTimeMS());
					} else {
						xSetInt(dFrostGiants, xSpecialNext, trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dFrostGiants, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dFrostGiants, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dFrostGiants, xSpecialStep) == 1) {
				xSetInt(dFrostGiants, xSpecialStep, 0);
				xSetInt(dFrostGiants, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount(dBarrages) > 0) {
		xDatabaseNext(dBarrages);
		if (trTimeMS() > xGetInt(dBarrages, xTimeout)) {
			p = xGetInt(dBarrages,xPlayerOwner);
			db = opponentDatabaseName(p);
			xSetInt(dBarrages, xTimeout, 300 + xGetInt(dBarrages, xTimeout));
			/* do the damage */
			pos = xGetVector(dBarrages,xBarragePos);
			dir = xGetVector(dBarrages,xBarrageDir);
			if (xGetInt(dBarrages, xBarrageCount) <= 3) {
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					id = xGetInt(db,xUnitID);
					xUnitSelectByID(db,xUnitID);
					if (trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 9.0) {
						damageOpponentUnit(p,100);
					}
				}
			}
			
			end = xsVectorSet(xsVectorGetX(pos) + 2.0 * xsVectorGetX(dir),0,xsVectorGetZ(pos) + 2.0 * xsVectorGetZ(dir));
			if (xGetInt(dBarrages, xBarrageCount) >= 0) {
				trArmyDispatch("1,0","Dwarf",1,xsVectorGetX(end),0,xsVectorGetZ(end),0,true);
				trArmySelect("1,0");
				trSetUnitOrientation(dir,vector(0,1,0),true);
				trUnitConvert(0);
				trUnitChangeProtoUnit("Barrage");
			}
			
			
			xSetInt(dBarrages, xBarrageCount, xGetInt(dBarrages, xBarrageCount) - 1);
			
			loc = vectorToGrid(end);
			if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL) || xGetInt(dBarrages, xBarrageCount) <= -2) {
				xFreeDatabaseBlock(dBarrages);
			} else {
				xSetVector(dBarrages,xBarragePos,end);
			}
		}
	}
	
	if(xGetDatabaseCount(dSatyrs) >0) {
		xDatabaseNext(dSatyrs);
		id = xGetInt(dSatyrs,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dSatyrs,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dSatyrs)) {
			xFreeDatabaseBlock(dSatyrs);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dSatyrs, xSpecialIndex)) == 1) {
			xSetInt(dSatyrs, xSpecialNext, trTimeMS() + 10000);
		} else if (trTimeMS() > xGetInt(dSatyrs, xSpecialNext)) {
			if (kbUnitGetAnimationActionType(id) == 12) {
				xsSetContextPlayer(p);
				target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
				xsSetContextPlayer(0);
				xSetInt(dSatyrs, xSpecialNext, trTimeMS() + 20000);
				end = kbGetBlockPosition(""+target);
				start = kbGetBlockPosition(""+xGetInt(dSatyrs,xUnitName));
				dir = getUnitVector(start,end);
				
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("rainofarrows"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				
				pos = xsVectorSet(xsVectorGetX(end) - 8.0 * xsVectorGetX(dir),0,xsVectorGetZ(end) - 8.0 * xsVectorGetZ(dir));
				xSetPointer(dBarrages, xAddDatabaseBlock(dBarrages));
				xSetVector(dBarrages,xBarrageDir,dir);
				xSetVector(dBarrages,xBarragePos,pos);
				xSetInt(dBarrages,xTimeout,trTimeMS());
				xSetInt(dBarrages,xBarrageCount,5);
				xSetInt(dBarrages,xPlayerOwner,p);
			}
		}
	}
	
	if(xGetDatabaseCount(dBattleBoars) >0) {
		xDatabaseNext(dBattleBoars);
		id = xGetInt(dBattleBoars,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dBattleBoars,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dBattleBoars)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Battle Boar");
			}
			xFreeDatabaseBlock(dBattleBoars);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dBattleBoars, xSpecialIndex)) == 1) {
			xSetInt(dBattleBoars, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dBattleBoars, xSpecialNext)) {
			switch(xGetInt(dBattleBoars, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetInt(dBattleBoars, xSpecialTarget, target);
						xSetInt(dBattleBoars, xSpecialStep, 1);
						xSetInt(dBattleBoars, xSpecialNext, trTimeMS() + 450);
						trUnitOverrideAnimation(26,0,false,false,-1);
					}
				}
				case 1:
				{
					start = kbGetBlockPosition(""+xGetInt(dBattleBoars,xUnitName));
					end = kbGetBlockPosition(""+xGetInt(dBattleBoars,xSpecialTarget));
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (unitDistanceToVector(xGetInt(db,xUnitName), end) < 9.0) {
							pos = vectorSetAsTargetVector(start, kbGetBlockPosition(""+xGetInt(db,xUnitName)), 20.0);
							launchUnit(db, pos);
						}
					}
					xSetInt(dBattleBoars, xSpecialStep, 2);
					xSetInt(dBattleBoars, xSpecialNext, xGetInt(dBattleBoars, xSpecialNext) + 300);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt(dBattleBoars, xSpecialStep, 0);
					xSetInt(dBattleBoars, xSpecialNext, trTimeMS() + 15000);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dBattleBoars, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dBattleBoars, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dBattleBoars, xSpecialStep) == 1) {
				xSetInt(dBattleBoars, xSpecialStep, 0);
				xSetInt(dBattleBoars, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount(dAvengerProj) > 0) {
		if (processGenericProj(dAvengerProj) == PROJ_FALLING) {
			p = xGetInt(dAvengerProj,xPlayerOwner);
			db = opponentDatabaseName(p);
			pos = xGetVector(dAvengerProj,xProjPrev);
			dir = xGetVector(dAvengerProj,xProjDir);
			end = kbGetBlockPosition(""+xGetInt(dAvengerProj,xUnitName));
			amt = distanceBetweenVectors(pos,end,false);
			if (amt > 2.0) {
				xSetFloat(dAvengerProj, xProjDist, xGetFloat(dAvengerProj, xProjDist) + amt);
				for(x=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					xUnitSelectByID(db,xUnitID);
					if (trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (rayCollision(db,pos,dir,amt + 1.0,9.0)) {
						damageOpponentUnit(p, amt * 20.0);
						silenceUnit(db,5.0,p);
					}
				}
				xSetVector(dAvengerProj,xProjPrev,end);
			}
			end = xsVectorSet(xsVectorGetX(end) + 4.0 * xsVectorGetX(dir),0,xsVectorGetZ(end) + 4.0 * xsVectorGetZ(dir));
			loc = vectorToGrid(end);
			if (xGetFloat(dAvengerProj, xProjDist) >= xGetFloat(dAvengerProj, xAvengerProjDist) ||
				terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				db = databaseName(p);
				xUnitSelectByID(dAvengerProj,xUnitID);
				trDamageUnitPercent(-100);
				trUnitChangeProtoUnit("Transport Ship Greek");
				xUnitSelectByID(dAvengerProj,xUnitID);
				trDamageUnitPercent(-100);
				xUnitSelect(dAvengerProj,xAvengerProjUnit);
				if (trUnitAlive()) {
					trImmediateUnitGarrison(""+xGetInt(dAvengerProj,xUnitName));
					trUnitChangeProtoUnit("Avenger");
					xSetInt(dAvengers,xSpecialStep,2,xGetInt(dAvengerProj,xAvengerProjIndex));
				}
				xUnitSelectByID(dAvengerProj,xUnitID);
				trUnitChangeProtoUnit("Dust Small");
				
				xFreeDatabaseBlock(dAvengerProj);
			}
		}
	}
	
	if(xGetDatabaseCount(dAvengers) >0) {
		xDatabaseNext(dAvengers);
		id = xGetInt(dAvengers,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dAvengers,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dAvengers)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Avenger");
			}
			xFreeDatabaseBlock(dAvengers);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dAvengers, xSpecialIndex)) == 1) {
			xSetInt(dAvengers, xSpecialNext, trTimeMS() + 10000);
		} else if (trTimeMS() > xGetInt(dAvengers, xSpecialNext)) {
			switch(xGetInt(dAvengers,xSpecialStep))
			{
				case 0:
				{
					xsSetContextPlayer(p);
					if ((kbUnitGetActionType(id) == 6) || (kbUnitGetActionType(id) == 48)) {
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						end = kbGetBlockPosition(""+target);
						start = kbGetBlockPosition(""+xGetInt(dAvengers,xUnitName));
						if (distanceBetweenVectors(start, end) < 144) {
							trSoundPlayFN("sphinxspecialattack.wav","1",-1,"","");
							dir = getUnitVector(start,end);
							trSetSelectedScale(0,0,0);
							addGenericProj(dAvengerProj,start,dir,p);
							xSetVector(dAvengerProj,xProjPrev,start);
							xSetFloat(dAvengerProj,xAvengerProjDist,distanceBetweenVectors(start, end,false) + 5.0);
							xSetInt(dAvengerProj,xAvengerProjUnit,xGetInt(dAvengers,xUnitName));
							xSetInt(dAvengerProj,xAvengerProjIndex,xGetPointer(dAvengers));
							
							xSetInt(dAvengers,xSpecialStep, 1);
							xSetInt(dAvengers, xSpecialNext, trTimeMS() + 3000);
							xSetInt(dAvengers,xSpecialTarget,xGetNewestPointer(dAvengerProj));
						}
					} else {
						xsSetContextPlayer(0);
					}
				}
				case 2:
				{
					xSetInt(dAvengers,xSpecialStep,0);
					xSetInt(dAvengers,xSpecialNext, trTimeMS() + 10000);
				}
			}
		}
	}
	
	if (xGetDatabaseCount(dBallistas) > 0) {
		xDatabaseNext(dBallistas);
		xUnitSelectByID(dBallistas,xUnitID);
		if (trUnitAlive() == false || checkEnemyDeactivated(dBallistas)) {
			xFreeDatabaseBlock(dBallistas);
		}
	}
	
	if (xGetDatabaseCount(dFireSiphons) > 0) {
		xDatabaseNext(dFireSiphons);
		id = xGetInt(dFireSiphons,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (trUnitAlive() == false || checkEnemyDeactivated(dFireSiphons)) {
			xFreeDatabaseBlock(dFireSiphons);
		} else if ((kbUnitGetAnimationActionType(id) == 9) && (xGetInt(dFireSiphons,xPlayerOwner) == ENEMY_PLAYER)) {
			if (trTimeMS() > xGetInt(dFireSiphons, xSpecialStep)) {
				xSetInt(dFireSiphons, xSpecialStep, trTimeMS() + 3000);
				xDatabaseNext(dPlayerUnits);
				pos = xGetVector(dPlayerUnits,xUnitPos);
				trUnitMoveToPoint(xsVectorGetX(pos),0,xsVectorGetZ(pos),-1,true);
			}
		}
	}
	
	if (xGetDatabaseCount(dMummyBalls) > 0) {
		if (processGenericProj(dMummyBalls) == PROJ_FALLING) {
			p = xGetInt(dMummyBalls,xPlayerOwner);
			db = opponentDatabaseName(p);
			pos = xGetVector(dMummyBalls,xProjPrev);
			dir = xGetVector(dMummyBalls,xProjDir);
			end = kbGetBlockPosition(""+xGetInt(dMummyBalls,xUnitName));
			amt = distanceBetweenVectors(end, pos, false);
			xSetVector(dMummyBalls, xProjPrev, end);
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db,xUnitID);
				if (trUnitAlive() == false) {
					removeOpponentUnit(p);
				} else if (rayCollision(db,pos,dir,amt + 4.0,xGetFloat(dMummyBalls,xProjDist))) {
					damageOpponentUnit(p, xsMin(100.0, amt * 10));
					switch(xGetInt(dMummyBalls, xProjType))
					{
						case STATUS_SILENCE:
						{
							silenceUnit(db, 5.0, p);
						}
						case STATUS_POISON:
						{
							poisonUnit(db, 10, 5.0 * trQuestVarGet("stage"), p);
						}
					}
				}
			}
			loc = vectorToGrid(end);
			if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				xFreeDatabaseBlock(dMummyBalls);
			}
		}
	}
	
	if (xGetDatabaseCount(dMummies) >0) {
		xDatabaseNext(dMummies);
		id = xGetInt(dMummies,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dMummies, xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dMummies)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Mummy");
			}
			xFreeDatabaseBlock(dMummies);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dMummies, xSpecialIndex)) == 1) {
			xSetInt(dMummies, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dMummies, xSpecialNext)) {
			switch(xGetInt(dMummies, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 12) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						end = kbGetBlockPosition(""+target);
						start = kbGetBlockPosition(""+xGetInt(dMummies,xUnitName));
						dir = getUnitVector(start,end);
						xSetVector(dMummies,xMummyStart,start);
						xSetVector(dMummies,xMummyDir,dir);
						xSetInt(dMummies, xSpecialStep, 1);
						xSetInt(dMummies, xSpecialNext, trTimeMS() + 1000);
						trUnitOverrideAnimation(37,0,false,false,-1);
					}
				}
				case 1:
				{
					start = xGetVector(dMummies,xMummyStart);
					dir = xGetVector(dMummies,xMummyDir);
					addGenericProj(dMummyBalls,start,dir,p);
					xSetInt(dMummyBalls, xProjProto, kbGetProtoUnitID("Lampades Blood"));
					xSetVector(dMummyBalls,xProjPrev,start);
					xSetFloat(dMummyBalls,xProjDist,16);
					xSetInt(dMummyBalls,xProjType,STATUS_POISON);
					xSetInt(dMummies, xSpecialStep, 2);
					xSetInt(dMummies, xSpecialNext, xGetInt(dMummies, xSpecialNext) + 3000);
				}
				case 2:
				{
					xSetInt(dMummies, xSpecialStep, 0);
					xSetInt(dMummies, xSpecialNext, trTimeMS() + 18000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dMummies, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dMummies, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dMummies, xSpecialStep) == 1) {
				xSetInt(dMummies, xSpecialStep, 0);
				xSetInt(dMummies, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if(xGetDatabaseCount(dScorpionMen) >0) {
		xDatabaseNext(dScorpionMen);
		id = xGetInt(dScorpionMen,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dScorpionMen,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dScorpionMen)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Scorpion Man");
			}
			xFreeDatabaseBlock(dScorpionMen);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dScorpionMen, xSpecialIndex)) == 1) {
			xSetInt(dScorpionMen, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dScorpionMen, xSpecialNext)) {
			switch(xGetInt(dScorpionMen, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetInt(dScorpionMen, xSpecialTarget, target);
						xSetInt(dScorpionMen, xSpecialStep, 1);
						xSetInt(dScorpionMen, xSpecialNext, trTimeMS() + 500);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					action = 0;
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						if (xGetInt(dScorpionMen, xSpecialTarget) == xGetInt(db,xUnitName)) {
							xUnitSelectByID(db,xUnitID);
							poisonUnit(db, 10, 5.0 * trQuestVarGet("stage"), p);
							action = 1;
							break;
						}
					}
					xSetInt(dScorpionMen, xSpecialStep, 2);
					xSetInt(dScorpionMen, xSpecialNext, xGetInt(dScorpionMen, xSpecialNext) + 500);
					if (action == 0) {
						xSetInt(dScorpionMen, xSpecialTarget, -1);
					}
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt(dScorpionMen, xSpecialStep, 0);
					if (xGetInt(dScorpionMen, xSpecialTarget) == -1) {
						xSetInt(dScorpionMen, xSpecialNext, trTimeMS());
					} else {
						xSetInt(dScorpionMen, xSpecialNext, trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dScorpionMen, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dScorpionMen, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dScorpionMen, xSpecialStep) == 1) {
				xSetInt(dScorpionMen, xSpecialStep, 0);
				xSetInt(dScorpionMen, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount(dHydras) > 0) {
		xDatabaseNext(dHydras);
		xUnitSelectByID(dHydras,xUnitID);
		if (trUnitAlive() == false || checkEnemyDeactivated(dHydras)) {
			xFreeDatabaseBlock(dHydras);
		} else if (trTime() > xGetInt(dHydras,xSpecialStep)) {
			amt = trTime() - xGetInt(dHydras, xSpecialStep);
			xSetInt(dHydras,xSpecialStep, trTime());
			db = databaseName(xGetInt(dHydras,xPlayerOwner));
			if (xGetInt(db,xPoisonStatus,xGetInt(dHydras,xSpecialIndex)) == 0) {
				xUnitSelectByID(dHydras,xUnitID);
				trDamageUnitPercent(0 - amt);
			}
		}
	}
	
	if(xGetDatabaseCount(dNereids) >0) {
		xDatabaseNext(dNereids);
		id = xGetInt(dNereids,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dNereids,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dNereids)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Nereid");
			}
			xFreeDatabaseBlock(dNereids);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dNereids, xSpecialIndex)) == 1) {
			xSetInt(dNereids, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dNereids, xSpecialNext)) {
			switch(xGetInt(dNereids, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						pos = kbGetBlockPosition(""+target);
						xSetVector(dNereids,xSpecialTarget,pos);
						xSetInt(dNereids, xSpecialStep, 1);
						xSetInt(dNereids, xSpecialNext, trTimeMS() + 1400);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					start = kbGetBlockPosition(""+xGetInt(dNereids,xUnitName));
					end = xGetVector(dNereids,xSpecialTarget);
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (unitDistanceToVector(xGetInt(db,xUnitName), end) < 16.0) {
							pos = vectorSetAsTargetVector(start, kbGetBlockPosition(""+xGetInt(db,xUnitName)), 40.0);
							damageOpponentUnit(p, 200.0);
							if (trUnitAlive()) {
								launchUnit(db, pos);
							}
						}
					}
					xSetInt(dNereids, xSpecialStep, 2);
					xSetInt(dNereids, xSpecialNext, xGetInt(dNereids, xSpecialNext) + 1600);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt(dNereids, xSpecialStep, 0);
					xSetInt(dNereids, xSpecialNext, trTimeMS() + 15000);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dNereids, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dNereids, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dNereids, xSpecialStep) == 1) {
				xSetInt(dNereids, xSpecialStep, 0);
				xSetInt(dNereids, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if(xGetDatabaseCount(dKrakens) >0) {
		xDatabaseNext(dKrakens);
		id = xGetInt(dKrakens,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dKrakens,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false) {
			trUnitChangeProtoUnit("Kraken");
			xFreeDatabaseBlock(dKrakens);
		} else if (checkEnemyDeactivated(dKrakens)) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock(dKrakens);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dKrakens, xSpecialIndex)) == 1) {
			xSetInt(dKrakens, xSpecialStep, 0);
		} else if (trTimeMS() > xGetInt(dKrakens, xSpecialNext)) {
			switch(xGetInt(dKrakens, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						end = kbGetBlockPosition(""+target);
						start = kbGetBlockPosition(""+xGetInt(dKrakens,xUnitName));
						dir = getUnitVector(start, end);
						xSetVector(dKrakens,xSpecialTarget,dir);
						xSetInt(dKrakens, xSpecialStep, 1);
						xSetInt(dKrakens, xSpecialNext, trTimeMS() + 2100);
						trUnitOverrideAnimation(1,0,false,false,-1);
					}
				}
				case 1:
				{
					db = opponentDatabaseName(p);
					dir = xGetVector(dKrakens,xSpecialTarget);
					start = kbGetBlockPosition(""+xGetInt(dKrakens,xUnitName));
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (rayCollision(db, start,dir, 6.0, 3.0)) {
							damageOpponentUnit(p, 300.0);
							stunUnit(db, 2.0);
						}
					}
					xSetInt(dKrakens, xSpecialStep, 2);
					xSetInt(dKrakens, xSpecialNext, xGetInt(dKrakens, xSpecialNext) + 1500);
				}
				case 2:
				{
					xSetInt(dKrakens, xSpecialStep, 0);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else if (xGetInt(dKrakens, xSpecialStep) != 0) {
			trSetUnitOrientation(xGetVector(dKrakens,xSpecialTarget),vector(0,1,0),true);
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dKrakens, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dKrakens, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dKrakens, xSpecialStep) == 1) {
				xSetInt(dKrakens, xSpecialStep, 0);
			}
		}
	}
	
	if(xGetDatabaseCount(dEinherjars) >0) {
		xDatabaseNext(dEinherjars);
		id = xGetInt(dEinherjars,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dEinherjars,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dEinherjars)) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Einheriar");
			}
			xFreeDatabaseBlock(dEinherjars);
		} else if (trTimeMS() > xGetInt(dEinherjars, xSpecialNext)) {
			switch(xGetInt(dEinherjars, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xSetInt(dEinherjars, xSpecialStep, 1);
						xSetInt(dEinherjars, xSpecialNext, trTimeMS() + 1400);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					start = kbGetBlockPosition(""+xGetInt(dEinherjars,xUnitName),true);
					for (x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeAllyUnit(p);
						} else if (xGetInt(db, xPoisonStatus) == 0) {
							pos = kbGetBlockPosition(""+xGetInt(db,xUnitName),true);
							if (distanceBetweenVectors(pos, start) < 9.0) {
								trDamageUnit(-100);
								trArmyDispatch("0,0","Dwarf",1,xsVectorGetX(pos),0,xsVectorGetZ(pos),0,true);
								trArmySelect("0,0");
								trUnitChangeProtoUnit("Regeneration SFX");
							}
						}
					}
					xSetInt(dEinherjars, xSpecialStep, 2);
					xSetInt(dEinherjars, xSpecialNext, xGetInt(dEinherjars, xSpecialNext) + 2600);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt(dEinherjars, xSpecialStep, 0);
					xSetInt(dEinherjars, xSpecialNext, trTimeMS() + 15000);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dEinherjars, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dEinherjars, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dEinherjars, xSpecialStep) == 1) {
				xSetInt(dEinherjars, xSpecialStep, 0);
				xSetInt(dEinherjars, xSpecialNext, trTimeMS() + 15000);
			}
		}
	}
	
	if (xGetDatabaseCount(dYeebLightningEnd) > 0) {
		trQuestVarSetFromRand("sound", 1, 5, true);
		hit = false;
		for(y=xGetDatabaseCount(dYeebLightningEnd); >0) {
			xDatabaseNext(dYeebLightningEnd);
			p = xGetInt(dYeebLightningEnd,xPlayerOwner);
			db = opponentDatabaseName(p);
			xUnitSelect(dYeebLightningEnd,xUnitName);
			if (trUnitVisToPlayer()) {
				hit = true;
			}
			pos = kbGetBlockPosition(""+xGetInt(dYeebLightningEnd,xUnitName));
			trUnitChangeProtoUnit("Lightning sparks");
			for(x=xGetDatabaseCount(db); >0) {
				xDatabaseNext(db);
				xUnitSelectByID(db,xUnitID);
				if (trUnitAlive() == false) {
					removeOpponentUnit(p);
				} else if (unitDistanceToVector(xGetInt(db,xUnitName), pos) < 1.0) {
					damageOpponentUnit(p, 470);
					if (boss > 0) {
						if (xGetBool(db, xIsHero)) {
							gainFavor(xGetInt(db, xPlayerOwner), -5.0);
						}
					}
				}
			}
			if (boss > 999) {
				for(x=xGetDatabaseCount(dEnemies); >0) {
					xDatabaseNext(dEnemies);
					xUnitSelectByID(dEnemies,xUnitID);
					if (trUnitAlive() == false) {
						removeEnemy();
					} else if (unitDistanceToVector(xGetInt(dEnemies,xUnitName), pos) < 1.0) {
						damageEnemy(0, 470, true);
						damageEnemy(0, 470, false);
					}
				}
			}
		}
		xClearDatabase(dYeebLightningEnd);
		if (hit) {
			trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	
	for(x=xsMin(10, xGetDatabaseCount(dYeebLightning)); >0) {
		xDatabaseNext(dYeebLightning);
		if (trTimeMS() > xGetInt(dYeebLightning, xTimeout)) {
			hit = true;
			trChatSetStatus(false);
			trDelayedRuleActivation("enable_chat");
			xSetPointer(dYeebLightningEnd,xAddDatabaseBlock(dYeebLightningEnd));
			xSetInt(dYeebLightningEnd,xUnitName,xGetInt(dYeebLightning,xUnitName));
			xSetInt(dYeebLightningEnd,xPlayerOwner,xGetInt(dYeebLightning,xPlayerOwner));
			xUnitSelect(dYeebLightning,xUnitName);
			trUnitChangeProtoUnit("Militia");
			xUnitSelect(dYeebLightning,xUnitName);
			trSetSelectedScale(0,0,0);
			trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
			xFreeDatabaseBlock(dYeebLightning);
		}
	}
	
	for(y=xsMin(4, xGetDatabaseCount(dYeebLightningBalls)); >0) {
		action = processGenericProj(dYeebLightningBalls);
		p = xGetInt(dYeebLightningBalls,xPlayerOwner);
		db = opponentDatabaseName(p);
		if ((action == PROJ_FALLING) || (action == PROJ_GROUND)) {
			dir = xGetVector(dYeebLightningBalls,xProjDir);
			start = xGetVector(dYeebLightningBalls,xProjPrev);
			pos = kbGetBlockPosition(""+xGetInt(dYeebLightningBalls,xUnitName),true);
			end = start + (dir * 3.0);
			loc = vectorToGrid(end);
			if (terrainIsType(loc, TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				if (xGetInt(dYeebLightningBalls, xProjDist) > 0) {
					xSetInt(dYeebLightningBalls, xProjDist, xGetInt(dYeebLightningBalls,xProjDist) - 1);
					dir = getBounceDir(end,loc,dir);
					xSetInt(dYeebLightningBalls, xProjYeehaw, 99);
					xSetVector(dYeebLightningBalls,xProjDir,dir);
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("implodehit"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				} else {
					xFreeDatabaseBlock(dYeebLightningBalls);
				}
			} else {
				dist = distanceBetweenVectors(pos,start);
				if (dist > 4.0) {
					dist = xsSqrt(dist) + 3.0;
					for(x=xGetDatabaseCount(db); >0) {
						xDatabaseNext(db);
						xUnitSelectByID(db,xUnitID);
						if (trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (rayCollision(db, start, dir, dist, 9.0)) {
							damageOpponentUnit(p, 120);
							stunUnit(db, 3.0, p, false);
							if (boss > 0) {
								if (xGetBool(db, xIsHero)) {
									gainFavor(xGetInt(db,xPlayerOwner), -1.0);
								}
							}
						}
					}
					xSetVector(dYeebLightningBalls,xProjPrev,pos);
				}
			}
		}
	}
	
	if(xGetDatabaseCount(dLightningStatues) >0) {
		xDatabaseNext(dLightningStatues);
		id = xGetInt(dLightningStatues,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dLightningStatues,xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false || checkEnemyDeactivated(dLightningStatues)) {
			xFreeDatabaseBlock(dLightningStatues);
		} else if (trTimeMS() > xGetInt(dLightningStatues, xSpecialNext)) {
			if (kbUnitGetAnimationActionType(id) == 59) {
				xsSetContextPlayer(p);
				target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
				xsSetContextPlayer(0);
				pos = vectorSnapToGrid(kbGetBlockPosition(""+target));
				trUnitOverrideAnimation(2,0,false,true,-1);
				xSetInt(dLightningStatues,xSpecialNext,trTimeMS() + 1000);
				spawnLightning(pos, p);
				trSoundPlayFN("mirrortowerfire.wav","1",-1,"","");
			}
		}
	}
	
	if (xGetDatabaseCount(dLampades) >0) {
		xDatabaseNext(dLampades);
		id = xGetInt(dLampades,xUnitID);
		trUnitSelectClear();
		trUnitSelectByID(id);
		p = xGetInt(dLampades, xPlayerOwner);
		db = databaseName(p);
		if (trUnitAlive() == false) {
			trUnitChangeProtoUnit("Lampades");
			xFreeDatabaseBlock(dLampades);
		} else if (checkEnemyDeactivated(dLampades)) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock(dLampades);
		} else if (xGetInt(db, xSilenceStatus, xGetInt(dLampades, xSpecialIndex)) == 1) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xSetInt(dLampades, xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt(dLampades, xSpecialNext)) {
			switch(xGetInt(dLampades, xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 12) {
						xsSetContextPlayer(p);
						target = trGetUnitScenarioNameNumber(kbUnitGetTargetUnitID(id));
						xsSetContextPlayer(0);
						xSetVector(dLampades, xSpecialTarget, kbGetBlockPosition(""+target));
						
						xSetInt(dLampades, xSpecialStep, 1);
						xSetInt(dLampades, xSpecialNext, trTimeMS() + 500);
						trUnitOverrideAnimation(37,0,false,false,-1);
					}
				}
				case 1:
				{
					end = xGetVector(dLampades,xSpecialTarget);
					start = kbGetBlockPosition(""+xGetInt(dLampades, xUnitName));
					dir = getUnitVector(start,end);
					addGenericProj(dYeebLightningBalls,start,dir,p);
					xSetVector(dYeebLightningBalls,xProjPrev,start);
					xSetInt(dYeebLightningBalls,xProjDist,3); // bounces
					xSetInt(dLampades, xSpecialStep, 2);
					xSetInt(dLampades, xSpecialNext, xGetInt(dLampades, xSpecialNext) + 500);
				}
				case 2:
				{
					xSetInt(dLampades, xSpecialStep, 0);
					xSetInt(dLampades, xSpecialNext, trTimeMS() + 18000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt(dLampades, xSpecialIndex));
			if (xGetBool(db, xLaunched, xGetInt(dLampades, xSpecialIndex))) {
				action = action + 1;
			}
			if (action > 0 && xGetInt(dLampades, xSpecialStep) == 1) {
				xSetInt(dLampades, xSpecialStep, 0);
				xSetInt(dLampades, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
}

void enemiesAlways() {
	int p = 0;
	int id = 0;
	int target = 0;
	float angle = 0;
	float amt = 0;
	string protoName = "";
	vector pos = vector(0,0,0);
	vector dir = vector(0,0,0);
	/*
	Enemies incoming
	*/
	for (x=xsMin(5, xGetDatabaseCount(dEnemiesIncoming)); > 0) {
		xDatabaseNext(dEnemiesIncoming);
		xUnitSelect(dEnemiesIncoming,xUnitName);
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				activateEnemy(xGetInt(dEnemiesIncoming,xUnitName));
				xFreeDatabaseBlock(dEnemiesIncoming);
				break;
			}
		}
	}
	
	if (xGetDatabaseCount(dEnemies) > 0) {
		xDatabaseNext(dEnemies);
		id = kbGetBlockID(""+xGetInt(dEnemies,xUnitName));
		trUnitSelectClear();
		trUnitSelectByID(id);
		if (id == -1 || trUnitAlive() == false) {
			removeEnemy();
		} else if ((kbUnitGetAnimationActionType(id) == 9) &&
			(xGetBool(dEnemies, xLaunched) == false) &&
			(xGetInt(dEnemies, xStunStatus) + xGetInt(dEnemies, xPoisonStatus) + xGetInt(dEnemies, xSilenceStatus) == 0)) {
			/* if idle and out of LOS, we remove it */
			xSetInt(dEnemies, xMissingTimeout, 1 + xGetInt(dEnemies, xMissingTimeout));
			if (xGetInt(dEnemies, xMissingTimeout) >= 10) {
				for(p=1; < ENEMY_PLAYER) {
					if (trUnitHasLOS(p)) {
						xSetInt(dEnemies, xMissingTimeout, 0);
						break;
					}
				}
				if (xGetInt(dEnemies, xMissingTimeout) >= 10) {
					xAddDatabaseBlock(dEnemiesIncoming, true);
					xSetInt(dEnemiesIncoming,xUnitName,xGetInt(dEnemies,xUnitName));
					xFreeDatabaseBlock(dEnemies);
				}
			}
		} else {
			stunsAndPoisons(dEnemies);
			xSetVector(dEnemies,xUnitPos,kbGetBlockPosition(""+xGetInt(dEnemies,xUnitName)));
		}
	}
	
	/* ambush rooms */
	if (xGetDatabaseCount(dAmbushRooms) > 0) {
		xDatabaseNext(dAmbushRooms);
		pos = xGetVector(dAmbushRooms,xAmbushRoomPos);
		for(p=1; < ENEMY_PLAYER) {
			if (unitDistanceToVector(xGetInt(dPlayerData,xPlayerUnit,p), pos) < 100) {
				trQuestVarSetFromRand("rand", 1, trQuestVarGet("enemyProtoCount"),true);
				protoName = trStringQuestVarGet("enemyProto"+1*trQuestVarGet("rand"));
				trQuestVarSetFromRand("count", trQuestVarGet("stage"), 11, true);
				angle = 6.283185 / trQuestVarGet("count");
				amt = 57.29578 * angle;
				float cSin = 0.0 - xsSin(angle);
				float cCos = xsCos(angle);
				float heading = 0;
				dir = xsVectorSet(0, 0, 1);
				for(x=trQuestVarGet("count"); >0) {
					trQuestVarSetFromRand("dist", 6, 18, true);
					
					target = trGetNextUnitScenarioNameNumber();
					trArmyDispatch(""+ENEMY_PLAYER+",0",protoName,1,
						xsVectorGetX(pos) - trQuestVarGet("dist") * xsVectorGetX(dir),0,
						xsVectorGetZ(pos) - trQuestVarGet("dist") * xsVectorGetZ(dir),heading,true);
					activateEnemy(target);
					
					heading = heading + amt;
					dir = rotationMatrix(dir,cCos,cSin);
				}
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				trSoundPlayFN("wild.wav","1",-1,"","");
				xFreeDatabaseBlock(dAmbushRooms);
				break;
			}
		}
	}
}

