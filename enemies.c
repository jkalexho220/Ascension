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
	if ((xGetBool(dEnemies, xDirtyBit, index) == false) ||
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
	xSetInt(dEnemies, xBounty, trQuestVarGet("bounty"));
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
	setupProtounitBounty("Statue of Lightning", 0.8, 8, 0.05);
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
			trQuestVarGet("proto"+1*trQuestVarGet("class"+class+"proto")+"armor"), 8, 0);
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
	int id = 0;
	int p = xGetInt(dBallistaShots,xPlayerOwner);
	int db = opponentDatabaseName(p);
	for(x=xGetDatabaseCount(db); >0) {
		xDatabaseNext(db);
		id = xGetInt(db, xUnitID);
		if (id == -1 || trUnitAlive() == false) {
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
		trUnitSelectClear();
		trUnitSelectByID(id);
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
			ballistaShotPop();
		} else if ((xGetInt(dBallistaShots, xPlayerOwner) == ENEMY_PLAYER) || PvP) {
			action = 0;
			for(p=1; < ENEMY_PLAYER) {
				if (PvP && (p == xGetInt(dBallistaShots, xPlayerOwner))) {
					continue;
				}
				action = action + trCountUnitsInArea(""+xGetInt(dBallistaShots,xUnitName),p,"Unit",2);
			}
			if (action > 0) {
				ballistaShotPop();
			}
		} else if (trCountUnitsInArea(""+xGetInt(dBallistaShots,xUnitName),ENEMY_PLAYER,"Unit",2) > 0) {
			ballistaShotPop();
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
				db = opponentDatabaseName(p);
				for(y=xGetDatabaseCount(db); >0) {
					xDatabaseNext(db);
					id = xGetInt(db, xUnitID);
					dir = xGetVector(db,xLaserDir);
					if (id == -1 || trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (rayCollision(db,start,dir,xGetFloat(dDelayLasers,xLaserDist),1.0)) {
						damageOpponentUnit(p, 300);
					}
				}
			}
		} else {
			amt = 0.012 * (xGetFloat(dDelayLasers, xLaserNext) - trTimeMS());
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
						
						addGenericProj(dMummyBalls,start,dir,kbGetProtoUnitID("Kronny Birth SFX"),2,8,4.5,0,xGetInt(dMummies,xPlayerOwner));
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
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated(dSphinxes)) {
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
							if (xGetInt(db,xUnitID) == -1 || trUnitAlive() == false) {
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
		if (id == -1 || trUnitAlive() == false) {
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
						if (xGetInt(db,xUnitID) == -1 || trUnitAlive() == false) {
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
			action = action + xGetInt(db, xLaunched, xGetInt(dMountainGiants, xSpecialIndex));
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
			if (xGetInt(db,xUnitID) == -1 || trUnitAlive() == false) {
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
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated(dMedusas)) {
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
					addGenericProj(dMedusaBalls,start,dir,kbGetProtoUnitID("Curse SFX"),2,4,4.5,0,p);
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
			action = action + xGetInt(db, xLaunched, xGetInt(dMedusas, xSpecialIndex));
			if (action > 0 && xGetInt(dMedusas, xSpecialStep) == 1) {
				xSetInt(dMedusas, xSpecialStep, 0);
				xSetInt(dMedusas, xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount("Valkyries") > 0) {
		id = yDatabaseNext("Valkyries", true);
		p = xGetInt("Valkyries",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Valkyries")) {
			trUnitSelectClear();
			trUnitSelect(""+xGetInt("Valkyries", "sfx"), true);
			trUnitDestroy();
			xFreeDatabaseBlock("Valkyries");
		} else {
			id = yGetPointer(db);
			if (ySetPointer(db, xGetInt("Valkyries", xSpecialIndex))) {
				xSetInt(db, "magicResist", 1 - xGetInt(db, xSilenceStatus));
				if (xGetInt(db, xSilenceStatus) != xGetInt("Valkyries", "silenced")) {
					xSetInt("Valkyries", "silenced", xGetInt(db, xSilenceStatus));
					trUnitSelectClear();
					trUnitSelect(""+xGetInt("Valkyries", "sfx"), true);
					if (xGetInt(db, xSilenceStatus) == 1) {
						trMutateSelected(kbGetProtoUnitID("Cinematic Block"));
					} else {
						trMutateSelected(kbGetProtoUnitID("Vortex Finish Linked"));
					}
				}
				ySetPointer(db, id);
			}
		}
	}
	
	if (xGetDatabaseCount("Dryads") > 0) {
		id = yDatabaseNext("Dryads", true);
		p = xGetInt("Dryads", xPlayerOwner);
		if (id == -1 || trUnitAlive() == false) {
			if (xGetInt("dryads", xSilenceStatus) == 0) {
				db = opponentDatabaseName(p);
				yVarToVector("Dryads", "pos");
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Lampades Blood");
				for(x=xGetDatabaseCount(db); >0) {
					id = yDatabaseNext(db, true);
					if (id == -1 || trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (zDistanceToVectorSquared(db, "pos") < 16) {
						poisonUnit(db, 10.0, 5.0 * trQuestVarGet("stage"), p);
					}
				}
				if (p == ENEMY_PLAYER) {
					if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 2) {
						if (zDistanceToVectorSquared("p"+trCurrentPlayer()+"unit", "pos") < 16) {
							trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 3);
							xsEnableRule("nick_next_dialog");
						}
					}
				}
			}
			xFreeDatabaseBlock("Dryads");
		} else if (checkEnemyDeactivated("Dryads")) {
			xFreeDatabaseBlock("Dryads");
		} else {
			db = databaseName(p);
			trVectorSetUnitPos("pos", "Dryads");
			ySetVarFromVector("Dryads", "pos", "pos");
			xSetInt("Dryads", xSilenceStatus, xGetInt(db, xSilenceStatus, xGetInt("Dryads", xSpecialIndex)));
		}
	}
	
	if (xGetDatabaseCount("scarabs") > 0) {
		id = yDatabaseNext("scarabs", true);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("scarabs")) {
			if (xGetInt("scarabs", xSilenceStatus) == 0) {
				yVarToVector("scarabs", "pos");
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect("1,0");
				trUnitChangeProtoUnit("Pestilence SFX1");
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("posX"),0,trQuestVarGet("posZ"),0,true);
				trArmySelect("1,0");
				trUnitConvert(ENEMY_PLAYER);
				trUnitChangeProtoUnit("Victory Marker");
			}
			xFreeDatabaseBlock("scarabs");
		} else if (checkEnemyDeactivated("scarabs")) {
			xFreeDatabaseBlock("scarabs");
		} else {
			db = databaseName(xGetInt("scarabs",xPlayerOwner));
			trVectorSetUnitPos("pos", "scarabs");
			ySetVarFromVector("scarabs", "pos", "pos");
			xSetInt("scarabs", xSilenceStatus, xGetInt(db, xSilenceStatus, xGetInt("scarabs", xSpecialIndex)));
		}
	}
	
	if (xGetDatabaseCount("automatonBombs") > 0) {
		yDatabaseNext("automatonBombs");
		if (trTimeMS() > xGetInt("automatonBombs", "timeout")) {
			p = xGetInt("automatonBombs",xPlayerOwner);
			db = opponentDatabaseName(p);
			yVarToVector("automatonBombs", "pos");
			trUnitSelectClear();
			trUnitSelectByQV("automatonBombs", true);
			trDamageUnitPercent(-100);
			trUnitChangeProtoUnit("Tartarian Gate Flame");
			trUnitSelectClear();
			trUnitSelectByQV("automatonBombs", true);
			trDamageUnitPercent(-100);
			trSetSelectedScale(1.0,1.0,0.4);
			xFreeDatabaseBlock("automatonBombs");
			for(x=xGetDatabaseCount(db); >0) {
				if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
					removePlayerUnit();
				} else if (zDistanceToVectorSquared(db, "pos") < 16) {
					damageOpponentUnit(p, 200);
				}
			}
		}
	}
	
	if (xGetDatabaseCount("automatons") > 0) {
		if (yDatabaseNext("automatons", true) == -1 || trUnitAlive() == false) {
			if (xGetInt("automatons", xSilenceStatus) == 0) {
				trUnitHighlight(3.0, true);
				trSoundPlayFN("gatherpoint.wav","1",-1,"","");
				yAddToDatabase("automatonBombs", "automatons");
				yAddUpdateVar("automatonBombs", "timeout", trTimeMS() + 3000);
				yAddUpdateVar("automatonBombs", "posx", xGetInt("automatons", "posx"));
				yAddUpdateVar("automatonBombs", "posz", xGetInt("automatons", "posz"));
				yAddUpdateVar("automatonBombs", xPlayerOwner, xGetInt("automatons",xPlayerOwner));
			}
			xFreeDatabaseBlock("automatons");
		} else if (checkEnemyDeactivated("automatons")) {
			xFreeDatabaseBlock("automatons");
		} else {
			db = databaseName(xGetInt("automatons",xPlayerOwner));
			trVectorSetUnitPos("pos", "automatons");
			ySetVarFromVector("automatons", "pos", "pos");
			xSetInt("automatons", xSilenceStatus, xGetInt(db, xSilenceStatus, xGetInt("automatons", xSpecialIndex)));
		}
	}
	
	if(xGetDatabaseCount("frostGiants") >0) {
		id = yDatabaseNext("frostGiants", true);
		p = xGetInt("frostGiants",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false) {
			trQuestVarSet("giantKills", 1 + trQuestVarGet("giantKills"));
			if (trQuestVarGet("frostGiantHunt") == 1) {
				trQuestVarSet("frostGiantBones", 1 + trQuestVarGet("frostGiantBones"));
			}
			trUnitChangeProtoUnit("Frost Giant");
			xFreeDatabaseBlock("frostGiants");
		} else if (checkEnemyDeactivated("frostGiants")) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock("frostGiants");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("frostGiants", xSpecialIndex)) == 1) {
			xSetInt("frostGiants", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("frostGiants", "specialnext")) {
			switch(xGetInt("frostGiants", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						xSetInt("frostGiants", "target", trGetUnitScenarioNameNumber(target));
						xSetInt("frostGiants", xSpecialStep, 1);
						xSetInt("frostGiants", "specialnext", trTimeMS() + 1400);
						trUnitOverrideAnimation(40,0,false,false,-1);
					}
				}
				case 1:
				{
					action = 0;
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						if (xGetInt("frostGiants", "target") == yDatabaseNext(db)) {
							trUnitSelectClear();
							trUnitSelectByQV(db);
							stunUnit(db, 3.0, p);
							action = 1;
							break;
						}
					}
					xSetInt("frostGiants", xSpecialStep, 2);
					xSetInt("frostGiants", "specialnext", xGetInt("frostGiants", "specialnext") + 600);
					if (action == 0) {
						xSetInt("frostGiants", "target", -1);
					} else if (trQuestVarGet("p"+trCurrentPlayer()+"nickQuestProgress") == 3) {
						if (xGetInt("frostGiants", "target") == trQuestVarGet("p"+trCurrentPlayer()+"unit")) {
							trQuestVarSet("p"+trCurrentPlayer()+"nickQuestProgress", 4);
							xsEnableRule("nick_next_dialog");
						}
					}
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt("frostGiants", xSpecialStep, 0);
					if (xGetInt("frostGiants", "target") == -1) {
						xSetInt("frostGiants", "specialnext", trTimeMS());
					} else {
						xSetInt("frostGiants", "specialnext", trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("frostGiants", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("frostGiants", xSpecialIndex));
			if (action > 0 && xGetInt("frostGiants", xSpecialStep) == 1) {
				xSetInt("frostGiants", xSpecialStep, 0);
				xSetInt("frostGiants", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount("barrages") > 0) {
		yDatabaseNext("barrages");
		if (trTimeMS() > xGetInt("barrages", xSpecialNext)) {
			p = xGetInt("barrages",xPlayerOwner);
			db = opponentDatabaseName(p);
			xSetInt("barrages", xSpecialNext, 300 + xGetInt("barrages", xSpecialNext));
			/* do the damage */
			yVarToVector("barrages", "pos");
			yVarToVector("barrages", "dir");
			if (xGetInt("barrages", "count") <= 3) {
				for(x=xGetDatabaseCount(db); >0) {
					if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else if (zDistanceToVectorSquared(db, "pos") < 9.0) {
						damageOpponentUnit(p,100);
					}
				}
			}
			
			if (xGetInt("barrages", "count") >= 0) {
				trQuestVarSet("endx", trQuestVarGet("posx") + 4.0 * trQuestVarGet("dirx"));
				trQuestVarSet("endz", trQuestVarGet("posz") + 4.0 * trQuestVarGet("dirz"));
				trQuestVarSet(xSpecialNext, trGetNextUnitScenarioNameNumber());
				trArmyDispatch("1,0","Dwarf",1,trQuestVarGet("endx"),0,trQuestVarGet("endz"),0,true);
				trArmySelect("1,0");
				trSetUnitOrientation(trVectorQuestVarGet("dir"),vector(0,1,0),true);
				trUnitConvert(0);
				trUnitChangeProtoUnit("Barrage");
			}
			
			
			xSetInt("barrages", "count", xGetInt("barrages", "count") - 1);
			trQuestVarSet("posx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
			trQuestVarSet("posz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL) || xGetInt("barrages", "count") <= -2) {
				xFreeDatabaseBlock("barrages");
			} else {
				ySetVarFromVector("barrages", "pos", "pos");
			}
		}
	}
	
	if(xGetDatabaseCount("Satyrs") >0) {
		id = yDatabaseNext("Satyrs", true);
		p = xGetInt("satyrs",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Satyrs")) {
			xFreeDatabaseBlock("Satyrs");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("Satyrs", xSpecialIndex)) == 1) {
			xSetInt("Satyrs", "specialnext", trTimeMS() + 10000);
		} else if (trTimeMS() > xGetInt("Satyrs", "specialnext")) {
			if (kbUnitGetAnimationActionType(id) == 12) {
				xsSetContextPlayer(p);
				target = kbUnitGetTargetUnitID(id);
				xSetInt("Satyrs", "specialnext", trTimeMS() + 20000);
				trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
				trVectorSetUnitPos("start", "Satyrs");
				trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
				
				trQuestVarSetFromRand("sound", 1, 3, true);
				trSoundPlayFN("rainofarrows"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				
				yAddToDatabase("barrages", xSpecialNext);
				yAddUpdateVar("barrages", "dirx", trQuestVarGet("dirx"));
				yAddUpdateVar("barrages", "dirz", trQuestVarGet("dirz"));
				yAddUpdateVar("barrages", "posx", trQuestVarGet("endx") - 8.0 * trQuestVarGet("dirX"));
				yAddUpdateVar("barrages", "posz", trQuestVarGet("endz") - 8.0 * trQuestVarGet("dirZ"));
				yAddUpdateVar("barrages", xSpecialNext, trTimeMS());
				yAddUpdateVar("barrages", "count", 5);
				yAddUpdateVar("barrages", xPlayerOwner, p);
			}
		}
	}
	
	if(xGetDatabaseCount("battleBoars") >0) {
		id = yDatabaseNext("battleBoars", true);
		p = xGetInt("battleBoars",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("battleBoars")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Battle Boar");
			}
			xFreeDatabaseBlock("battleBoars");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("battleBoars", xSpecialIndex)) == 1) {
			xSetInt("battleBoars", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("battleBoars", "specialnext")) {
			switch(xGetInt("battleBoars", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						xSetInt("battleBoars", "target", trGetUnitScenarioNameNumber(target));
						xSetInt("battleBoars", xSpecialStep, 1);
						xSetInt("battleBoars", "specialnext", trTimeMS() + 450);
						trUnitOverrideAnimation(26,0,false,false,-1);
					}
				}
				case 1:
				{
					trVectorSetUnitPos("start", "battleBoars");
					trVectorQuestVarSet("end", kbGetBlockPosition(""+xGetInt("battleBoars", "target")));
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (zDistanceToVectorSquared(db, "end") < 9.0) {
							trVectorSetUnitPos("pos", db);
							vectorSetAsTargetVector("target", "start", "pos", 20.0);
							launchUnit(db, "target");
						}
					}
					xSetInt("battleBoars", xSpecialStep, 2);
					xSetInt("battleBoars", "specialnext", xGetInt("battleBoars", "specialnext") + 300);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt("battleBoars", xSpecialStep, 0);
					if (xGetInt("battleBoars", "target") == -1) {
						xSetInt("battleBoars", "specialnext", trTimeMS());
					} else {
						xSetInt("battleBoars", "specialnext", trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("battleBoars", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("battleBoars", xSpecialIndex));
			if (action > 0 && xGetInt("battleBoars", xSpecialStep) == 1) {
				xSetInt("battleBoars", xSpecialStep, 0);
				xSetInt("battleBoars", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount("AvengerProj") > 0) {
		if (processGenericProj("AvengerProj") == PROJ_FALLING) {
			p = xGetInt("AvengerProj",xPlayerOwner);
			db = opponentDatabaseName(p);
			yVarToVector("AvengerProj", "prev");
			yVarToVector("AvengerProj", "dir");
			trVectorSetUnitPos("pos", "AvengerProj");
			amt = zDistanceBetweenVectors("pos", "prev");
			if (amt > 2.0) {
				yVarToVector("AvengerProj", "dir");
				xSetInt("AvengerProj", "currentDist", xGetInt("AvengerProj", "currentDist") + amt);
				for(x=xGetDatabaseCount(db); >0) {
					if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
						removeOpponentUnit(p);
					} else {
						dist = zDistanceToVector(db, "prev");
						if (dist < amt + 1.0) {
							trQuestVarSet("hitboxX", trQuestVarGet("prevX") + dist * trQuestVarGet("dirX"));
							trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + dist * trQuestVarGet("dirZ"));
							if (zDistanceToVectorSquared(db, "hitbox") < 9.0) {
								damageOpponentUnit(p, amt * 20.0);
								silenceUnit(db,5.0,p);
							}
						}
					}
				}
				ySetVarFromVector("AvengerProj", "prev", "pos");
			}
			trQuestVarSet("nextx", trQuestVarGet("posx") + 2.0 * trQuestVarGet("dirx"));
			trQuestVarSet("nextz", trQuestVarGet("posz") + 2.0 * trQuestVarGet("dirz"));
			vectorToGrid(xSpecialNext, "loc");
			if (xGetInt("AvengerProj", "currentDist") >= xGetInt("AvengerProj", "distance") ||
				terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				db = databaseName(p);
				trUnitSelectClear();
				trUnitSelectByQV("AvengerProj", true);
				trDamageUnitPercent(-100);
				trUnitChangeProtoUnit("Transport Ship Greek");
				trUnitSelectClear();
				trUnitSelectByQV("AvengerProj", true);
				trDamageUnitPercent(-100);
				trUnitSelectClear();
				trUnitSelect(""+xGetInt("AvengerProj","unit"),true);
				if (trUnitAlive()) {
					trImmediateUnitGarrison(""+1*trQuestVarGet("AvengerProj"));
					trUnitChangeProtoUnit("Avenger");
					ySetVarAtIndex("Avengers",xSpecialStep,2,xGetInt("AvengerProj",xSpecialIndex));
				}
				trUnitSelectClear();
				trUnitSelectByQV("AvengerProj");
				trUnitChangeProtoUnit("Dust Small");
				
				xFreeDatabaseBlock("AvengerProj");
			}
		}
	}
	
	if(xGetDatabaseCount("Avengers") >0) {
		id = yDatabaseNext("Avengers", true);
		p = xGetInt("Avengers",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Avengers")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Avengers");
			}
			xFreeDatabaseBlock("Avengers");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("Avengers", xSpecialIndex)) == 1) {
			xSetInt("Avengers", "specialnext", trTimeMS() + 10000);
		} else if (trTimeMS() > xGetInt("Avengers", "specialnext")) {
			switch(xGetInt("Avengers",xSpecialStep))
			{
				case 0:
				{
					xsSetContextPlayer(p);
					if ((kbUnitGetActionType(id) == 6) || (kbUnitGetActionType(id) == 48)) {
						target = kbUnitGetTargetUnitID(id);
						trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
						trVectorSetUnitPos("start", "Avengers");
						if (zDistanceBetweenVectorsSquared("start", "end") < 144) {
							trSoundPlayFN("sphinxspecialattack.wav","1",-1,"","");
							trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
							trSetSelectedScale(0,0,0);
							addGenericProj("avengerProj","start","dir",kbGetProtoUnitID("Avenger"),39,10.0,4.5,1.0,p);
							yAddUpdateVar("avengerProj", "prevX", trQuestVarGet("startx"));
							yAddUpdateVar("avengerProj", "prevz", trQuestVarGet("startz"));
							yAddUpdateVar("avengerProj", "distance", zDistanceBetweenVectors("start","end") + 5.0);
							yAddUpdateVar("avengerProj", "currentDist", 0);
							yAddUpdateVar("avengerProj", "unit", trQuestVarGet("Avengers"));
							yAddUpdateVar("avengerProj", xSpecialIndex, yGetPointer("Avengers"));
							yAddUpdateVar("avengerProj", xPlayerOwner,p);
							xSetInt("Avengers",xSpecialStep, 1);
							xSetInt("Avengers", "specialnext", trTimeMS() + 3000);
							xSetInt("Avengers","proj",yGetNewestPointer("avengerProj"));
						}
					}
				}
				case 2:
				{
					xSetInt("Avengers",xSpecialStep,0);
					xSetInt("Avengers","specialNext", trTimeMS() + 10000);
				}
			}
		}
	}
	
	if (xGetDatabaseCount("ballistas") > 0) {
		id = yDatabaseNext("ballistas", true);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("ballistas")) {
			xFreeDatabaseBlock("ballistas");
		}
	}
	
	if (xGetDatabaseCount("siphons") > 0) {
		id = yDatabaseNext("siphons", true);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("siphons")) {
			xFreeDatabaseBlock("siphons");
		} else if ((kbUnitGetAnimationActionType(id) == 9) && (xGetInt("siphons",xPlayerOwner) == ENEMY_PLAYER)) {
			if (trTimeMS() > xGetInt("siphons", xSpecialNext)) {
				xSetInt("siphons", xSpecialNext, trTimeMS() + 3000);
				xDatabaseNext(dPlayerUnits);
				trVectorSetUnitPos("pos", "playerUnits");
				trUnitMoveToPoint(trQuestVarGet("posx"),0,trQuestVarGet("posz"),-1,true);
			}
		}
	}
	
	if (xGetDatabaseCount("MummyBalls") > 0) {
		if (processGenericProj("MummyBalls") == PROJ_FALLING) {
			p = xGetInt("MummyBalls",xPlayerOwner);
			db = opponentDatabaseName(p);
			yVarToVector("MummyBalls", "prev");
			yVarToVector("MummyBalls", "dir");
			amt = zDistanceBetweenVectors("pos", "prev");
			ySetVarFromVector("MummyBalls", "prev", "pos");
			for(x=xGetDatabaseCount(db); >0) {
				if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
					removeOpponentUnit(p);
				} else {
					dist = zDistanceToVector(db, "prev");
					if (dist < amt + 4.0) {
						trQuestVarSet("hitboxX", trQuestVarGet("prevX") + dist * trQuestVarGet("dirX"));
						trQuestVarSet("hitboxZ", trQuestVarGet("prevZ") + dist * trQuestVarGet("dirZ"));
						if (zDistanceToVectorSquared(db, "hitbox") < xGetInt("MummyBalls", "dist")) {
							damageOpponentUnit(p, xsMin(100.0, amt * 10));
							switch(xGetInt("MummyBalls", "type"))
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
				}
			}
			vectorToGrid("pos", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				xFreeDatabaseBlock("MummyBalls");
			}
		}
	}
	
	if (xGetDatabaseCount("Mummies") >0) {
		id = yDatabaseNext("Mummies", true);
		p = xGetInt("Mummies", xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Mummies")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Mummy");
			}
			xFreeDatabaseBlock("Mummies");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("Mummies", xSpecialIndex)) == 1) {
			xSetInt("Mummies", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("Mummies", xSpecialNext)) {
			switch(xGetInt("Mummies", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 12) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						trVectorQuestVarSet("end", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
						trVectorSetUnitPos("start", "Mummies");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
						ySetVarFromVector("Mummies", "dir", "dir");
						ySetVarFromVector("Mummies", "start", "start");
						xSetInt("Mummies", xSpecialStep, 1);
						xSetInt("Mummies", xSpecialNext, trTimeMS() + 1000);
						trUnitOverrideAnimation(37,0,false,false,-1);
					}
				}
				case 1:
				{
					yVarToVector("Mummies", "start");
					yVarToVector("Mummies", "dir");
					addGenericProj("MummyBalls","start","dir",kbGetProtoUnitID("Lampades Blood"),2,10,4.0);
					yAddUpdateVar("MummyBalls", "prevX", trQuestVarGet("startx"));
					yAddUpdateVar("MummyBalls", "prevZ", trQuestVarGet("startz"));
					yAddUpdateVar("MummyBalls", "dist", 16);
					yAddUpdateVar("MummyBalls", "type", STATUS_POISON);
					yAddUpdateVar("MummyBalls",xPlayerOwner,p);
					xSetInt("Mummies", xSpecialStep, 2);
					xSetInt("Mummies", xSpecialNext, xGetInt("Mummies", xSpecialNext) + 3000);
				}
				case 2:
				{
					xSetInt("Mummies", xSpecialStep, 0);
					xSetInt("Mummies", xSpecialNext, trTimeMS() + 18000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("Mummies", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("Mummies", xSpecialIndex));
			if (action > 0 && xGetInt("Mummies", xSpecialStep) == 1) {
				xSetInt("Mummies", xSpecialStep, 0);
				xSetInt("Mummies", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if(xGetDatabaseCount("ScorpionMen") >0) {
		id = yDatabaseNext("ScorpionMen", true);
		p = xGetInt("ScorpionMen",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("ScorpionMen")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Scorpion Man");
			}
			xFreeDatabaseBlock("ScorpionMen");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("ScorpionMen", xSpecialIndex)) == 1) {
			xSetInt("ScorpionMen", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("ScorpionMen", "specialnext")) {
			switch(xGetInt("ScorpionMen", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						xSetInt("ScorpionMen", "target", trGetUnitScenarioNameNumber(target));
						xSetInt("ScorpionMen", xSpecialStep, 1);
						xSetInt("ScorpionMen", "specialnext", trTimeMS() + 500);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					action = 0;
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						if (xGetInt("ScorpionMen", "target") == yDatabaseNext(db)) {
							trUnitSelectClear();
							trUnitSelectByQV(db);
							poisonUnit(db, 10, 5.0 * trQuestVarGet("stage"), p);
							action = 1;
							break;
						}
					}
					xSetInt("ScorpionMen", xSpecialStep, 2);
					xSetInt("ScorpionMen", "specialnext", xGetInt("ScorpionMen", "specialnext") + 500);
					if (action == 0) {
						xSetInt("ScorpionMen", "target", -1);
					}
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt("ScorpionMen", xSpecialStep, 0);
					if (xGetInt("ScorpionMen", "target") == -1) {
						xSetInt("ScorpionMen", "specialnext", trTimeMS());
					} else {
						xSetInt("ScorpionMen", "specialnext", trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("ScorpionMen", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("ScorpionMen", xSpecialIndex));
			if (action > 0 && xGetInt("ScorpionMen", xSpecialStep) == 1) {
				xSetInt("ScorpionMen", xSpecialStep, 0);
				xSetInt("ScorpionMen", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if (xGetDatabaseCount("hydras") > 0) {
		if (yDatabaseNext("hydras", true) == -1 || trUnitAlive() == false || checkEnemyDeactivated("Hydras")) {
			xFreeDatabaseBlock("hydras");
		} else if (trTime() > xGetInt("hydras","last")) {
			amt = trTime() - xGetInt("hydras", "last");
			xSetInt("hydras","last", trTime());
			trDamageUnitPercent(0 - amt);
		}
	}
	
	if(xGetDatabaseCount("Nereids") >0) {
		id = yDatabaseNext("Nereids", true);
		p = xGetInt("Nereids",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Nereids")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Nereid");
			}
			xFreeDatabaseBlock("Nereids");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("Nereids", xSpecialIndex)) == 1) {
			xSetInt("Nereids", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("Nereids", "specialnext")) {
			switch(xGetInt("Nereids", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						trVectorQuestVarSet("pos", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
						ySetVarFromVector("Nereids", "end", "pos");
						xSetInt("Nereids", xSpecialStep, 1);
						xSetInt("Nereids", "specialnext", trTimeMS() + 1400);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					trVectorSetUnitPos("start", "Nereids");
					yVarToVector("Nereids", "end");
					db = opponentDatabaseName(p);
					for (x=xGetDatabaseCount(db); >0) {
						if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (zDistanceToVectorSquared(db, "end") < 16.0) {
							trVectorSetUnitPos("pos", db);
							vectorSetAsTargetVector("target", "start", "pos", 40.0);
							damageOpponentUnit(p, 200.0);
							if (trUnitAlive()) {
								launchUnit(db, "target");
							}
						}
					}
					xSetInt("Nereids", xSpecialStep, 2);
					xSetInt("Nereids", "specialnext", xGetInt("Nereids", "specialnext") + 1600);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt("Nereids", xSpecialStep, 0);
					if (xGetInt("Nereids", "target") == -1) {
						xSetInt("Nereids", "specialnext", trTimeMS());
					} else {
						xSetInt("Nereids", "specialnext", trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("Nereids", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("Nereids", xSpecialIndex));
			if (action > 0 && xGetInt("Nereids", xSpecialStep) == 1) {
				xSetInt("Nereids", xSpecialStep, 0);
				xSetInt("Nereids", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
	
	if(xGetDatabaseCount("krakens") >0) {
		id = yDatabaseNext("krakens", true);
		p = xGetInt("krakens",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false) {
			trUnitChangeProtoUnit("Kraken");
			xFreeDatabaseBlock("krakens");
		} else if (checkEnemyDeactivated("krakens")) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock("krakens");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("krakens", xSpecialIndex)) == 1) {
			xSetInt("krakens", xSpecialStep, 0);
		} else if (trTimeMS() > xGetInt("krakens", "specialnext")) {
			switch(xGetInt("krakens", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						trVectorQuestVarSet("target", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
						trVectorSetUnitPos("start", "krakens");
						trVectorQuestVarSet("dir", zGetUnitVector("start", "target"));
						xSetInt("krakens", "dirx", trQuestVarGet("dirx"));
						xSetInt("krakens", "dirz", trQuestVarGet("dirz"));
						xSetInt("krakens", xSpecialStep, 1);
						xSetInt("krakens", "specialnext", trTimeMS() + 2100);
						trUnitOverrideAnimation(1,0,false,false,-1);
					}
				}
				case 1:
				{
					db = opponentDatabaseName(p);
					yVarToVector("krakens", "dir");
					trVectorSetUnitPos("start", "krakens");
					for (x=xGetDatabaseCount(db); >0) {
						if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (rayCollision(db, "start","dir", 6.0, 3.0)) {
							damageOpponentUnit(p, 300.0);
							stunUnit(db, 2.0);
						}
					}
					xSetInt("krakens", xSpecialStep, 2);
					xSetInt("krakens", "specialnext", xGetInt("krakens", "specialnext") + 1500);
				}
				case 2:
				{
					xSetInt("krakens", xSpecialStep, 0);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else if (xGetInt("krakens", xSpecialStep) != 0) {
			trSetUnitOrientation(xsVectorSet(xGetInt("krakens","dirx"),0,xGetInt("krakens","dirz")),vector(0,1,0),true);
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("krakens", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("krakens", xSpecialIndex));
			if (action > 0 && xGetInt("krakens", xSpecialStep) == 1) {
				xSetInt("krakens", xSpecialStep, 0);
			}
		}
	}
	
	if(xGetDatabaseCount("Einherjars") >0) {
		id = yDatabaseNext("Einherjars", true);
		p = xGetInt("Einherjars",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("Einherjars")) {
			if (trUnitAlive()) {
				trUnitOverrideAnimation(-1,0,false,true,-1);
			} else {
				trUnitChangeProtoUnit("Einheriar");
			}
			xFreeDatabaseBlock("Einherjars");
		} else if (trTimeMS() > xGetInt("Einherjars", "specialnext")) {
			switch(xGetInt("Einherjars", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 6) {
						xSetInt("Einherjars", xSpecialStep, 1);
						xSetInt("Einherjars", "specialnext", trTimeMS() + 1400);
						trUnitOverrideAnimation(39,0,false,false,-1);
					}
				}
				case 1:
				{
					trVectorSetUnitPos("start", "Einherjars");
					for (x=xGetDatabaseCount(db); >0) {
						if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
							removeAllyUnit(p);
						} else if (xGetInt(db, "poisonStatus") == 0) {
							trVectorSetUnitPos("pos", db);
							if (zDistanceBetweenVectorsSquared("pos", "start") < 9.0) {
								trDamageUnit(-100);
								trArmyDispatch("0,0","Dwarf",1,trQuestVarGet("posx"),0,trQuestVarGet("posz"),0,true);
								trArmySelect("0,0");
								trUnitChangeProtoUnit("Regeneration SFX");
							}
						}
					}
					xSetInt("Einherjars", xSpecialStep, 2);
					xSetInt("Einherjars", "specialnext", xGetInt("Einherjars", "specialnext") + 2600);
				}
				case 2:
				{
					trUnitOverrideAnimation(-1,0,false,true,-1);
					xSetInt("Einherjars", xSpecialStep, 0);
					if (xGetInt("Einherjars", "target") == -1) {
						xSetInt("Einherjars", "specialnext", trTimeMS());
					} else {
						xSetInt("Einherjars", "specialnext", trTimeMS() + 15000);
					}
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("Einherjars", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("Einherjars", xSpecialIndex));
			if (action > 0 && xGetInt("Einherjars", xSpecialStep) == 1) {
				xSetInt("Einherjars", xSpecialStep, 0);
				xSetInt("Einherjars", xSpecialNext, trTimeMS() + 15000);
			}
		}
	}
	
	if (xGetDatabaseCount("yeebLightningEnd") > 0) {
		trQuestVarSetFromRand("sound", 1, 5, true);
		hit = false;
		for(y=xGetDatabaseCount("yeebLightningEnd"); >0) {
			yDatabaseNext("yeebLightningEnd", true);
			p = xGetInt("yeebLightningEnd",xPlayerOwner);
			db = opponentDatabaseName(p);
			if (trUnitVisToPlayer()) {
				hit = true;
			}
			trVectorSetUnitPos("pos", "yeebLightningEnd");
			trUnitChangeProtoUnit("Lightning sparks");
			for(x=xGetDatabaseCount(db); >0) {
				if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
					removeOpponentUnit(p);
				} else if (zDistanceToVectorSquared(db, "pos") < 1.0) {
					damageOpponentUnit(p, 470);
					if (trQuestVarGet("boss") > 0) {
						if (xGetInt(db, "hero") == 1) {
							gainFavor(xGetInt(db, xPlayerOwner), -5.0);
						}
					}
				}
			}
			if (trQuestVarGet("boss") > 999) {
				for(x=xGetDatabaseCount(dEnemies); >0) {
					if (yDatabaseNext("enemies", true) == -1 || trUnitAlive() == false) {
						removeEnemy();
					} else if (zDistanceToVectorSquared("enemies", "pos") < 1.0) {
						damageEnemy(0, 470, true);
						damageEnemy(0, 470, false);
					}
				}
			}
		}
		yClearDatabase("yeebLightningEnd");
		if (hit) {
			trSoundPlayFN("lightningstrike"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
		}
	}
	
	for(x=xsMin(10, xGetDatabaseCount("yeebLightning")); >0) {
		yDatabaseNext("yeebLightning");
		if (trTimeMS() > xGetInt("yeebLightning", "timeout")) {
			hit = true;
			trChatSetStatus(false);
			trDelayedRuleActivation("enable_chat");
			yAddToDatabase("yeebLightningEnd", "yeebLightning");
			yAddUpdateVar("yeebLightningEnd",xPlayerOwner,xGetInt("yeebLightning",xPlayerOwner));
			trUnitSelectClear();
			trUnitSelectByQV("yeebLightning", true);
			trUnitChangeProtoUnit("Militia");
			trUnitSelectClear();
			trUnitSelectByQV("yeebLightning", true);
			trSetSelectedScale(0,0,0);
			trTechInvokeGodPower(0, "bolt", vector(0,0,0), vector(0,0,0));
			xFreeDatabaseBlock("yeebLightning");
		}
	}
	
	for(y=xsMin(4, xGetDatabaseCount("yeebLightningBalls")); >0) {
		action = processGenericProj("yeebLightningBalls");
		p = xGetInt("yeebLightningBalls",xPlayerOwner);
		db = opponentDatabaseName(p);
		if ((action == PROJ_FALLING) || (action == PROJ_GROUND)) {
			yVarToVector("yeebLightningBalls", "dir");
			yVarToVector("yeebLightningBalls", "prev");
			trQuestVarSet("destx", trQuestVarGet("dirx") * 3.0 + trQuestVarGet("posx"));
			trQuestVarSet("destz", trQuestVarGet("dirz") * 3.0 + trQuestVarGet("posz"));
			vectorToGrid("dest", "loc");
			if (terrainIsType("loc", TERRAIN_WALL, TERRAIN_SUB_WALL)) {
				if (xGetInt("yeebLightningBalls", "bounces") > 0) {
					xSetInt("yeebLightningBalls", "bounces", xGetInt("yeebLightningBalls","bounces") - 1);
					trVectorQuestVarSet("dir", getBounceDir("loc", "dir"));
					xSetInt("yeebLightningBalls", "yeehaw", 99);
					ySetVarFromVector("yeebLightningBalls", "dir", "dir");
					trQuestVarSetFromRand("sound", 1, 2, true);
					trSoundPlayFN("implodehit"+1*trQuestVarGet("sound")+".wav","1",-1,"","");
				} else {
					xFreeDatabaseBlock("yeebLightningBalls");
				}
			} else {
				dist = zDistanceBetweenVectorsSquared("pos", "prev");
				if (dist > 4.0) {
					dist = xsSqrt(dist) + 3.0;
					for(x=xGetDatabaseCount(db); >0) {
						if (yDatabaseNext(db, true) == -1 || trUnitAlive() == false) {
							removeOpponentUnit(p);
						} else if (rayCollision(db, "prev", "dir", dist, 9.0)) {
							damageOpponentUnit(p, 120);
							stunUnit(db, 3.0, p, false);
							if (trQuestVarGet("boss") > 0) {
								if (xGetInt(db, "hero") == 1) {
									gainFavor(xGetInt(db,xPlayerOwner), -1.0);
								}
							}
						}
					}
					ySetVarFromVector("yeebLightningBalls", "prev", "pos");
				}
			}
		}
	}
	
	if(xGetDatabaseCount("lightningStatues") >0) {
		id = yDatabaseNext("lightningStatues", true);
		p = xGetInt("lightningStatues",xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false || checkEnemyDeactivated("lightningStatues")) {
			xFreeDatabaseBlock("lightningStatues");
		} else if (trTimeMS() > xGetInt("lightningStatues", "specialnext")) {
			if (kbUnitGetAnimationActionType(id) == 59) {
				xsSetContextPlayer(p);
				target = kbUnitGetTargetUnitID(id);
				trVectorQuestVarSet("target", kbGetBlockPosition(""+trGetUnitScenarioNameNumber(target)));
				vectorSnapToGrid("target");
				trUnitOverrideAnimation(2,0,false,true,-1);
				xSetInt("lightningStatues","specialNext",trTimeMS() + 1000);
				spawnLightning("target", p);
				trSoundPlayFN("mirrortowerfire.wav","1",-1,"","");
			}
		}
	}
	
	if (xGetDatabaseCount("SkyWitches") >0) {
		id = yDatabaseNext("SkyWitches", true);
		p = xGetInt("SkyWitches", xPlayerOwner);
		db = databaseName(p);
		if (id == -1 || trUnitAlive() == false) {
			trUnitChangeProtoUnit("Lampades");
			xFreeDatabaseBlock("SkyWitches");
		} else if (checkEnemyDeactivated("SkyWitches")) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xFreeDatabaseBlock("SkyWitches");
		} else if (xGetInt(db, xSilenceStatus, xGetInt("SkyWitches", xSpecialIndex)) == 1) {
			trUnitOverrideAnimation(-1,0,false,true,-1);
			xSetInt("SkyWitches", xSpecialStep, 2);
		} else if (trTimeMS() > xGetInt("SkyWitches", xSpecialNext)) {
			switch(xGetInt("SkyWitches", xSpecialStep))
			{
				case 0:
				{
					if (kbUnitGetAnimationActionType(id) == 12) {
						xsSetContextPlayer(p);
						target = kbUnitGetTargetUnitID(id);
						xSetInt("SkyWitches", "target", trGetUnitScenarioNameNumber(target));
						
						xSetInt("SkyWitches", xSpecialStep, 1);
						xSetInt("SkyWitches", xSpecialNext, trTimeMS() + 500);
						trUnitOverrideAnimation(37,0,false,false,-1);
					}
				}
				case 1:
				{
					trVectorQuestVarSet("end", kbGetBlockPosition(""+xGetInt("SkyWitches", "target")));
					trVectorSetUnitPos("start", "SkyWitches");
					trVectorQuestVarSet("dir", zGetUnitVector("start", "end"));
					addGenericProj("yeebLightningBalls","start","dir",kbGetProtoUnitID("Arkantos God"),26,10,5);
					yAddUpdateVar("yeebLightningBalls", "prevx", trQuestVarGet("startx"));
					yAddUpdateVar("yeebLightningBalls", "prevz", trQuestVarGet("startz"));
					yAddUpdateVar("yeebLightningBalls", "bounces", 3);
					yAddUpdateVar("yeebLightningBalls",xPlayerOwner,p);
					xSetInt("SkyWitches", xSpecialStep, 2);
					xSetInt("SkyWitches", xSpecialNext, xGetInt("SkyWitches", xSpecialNext) + 500);
				}
				case 2:
				{
					xSetInt("SkyWitches", xSpecialStep, 0);
					xSetInt("SkyWitches", xSpecialNext, trTimeMS() + 18000);
					trUnitOverrideAnimation(-1,0,false,true,-1);
				}
			}
		} else {
			action = xGetInt(db, xStunStatus, xGetInt("SkyWitches", xSpecialIndex));
			action = action + xGetInt(db, xLaunched, xGetInt("SkyWitches", xSpecialIndex));
			if (action > 0 && xGetInt("SkyWitches", xSpecialStep) == 1) {
				xSetInt("SkyWitches", xSpecialStep, 0);
				xSetInt("SkyWitches", xSpecialNext, trTimeMS() + 18000);
			}
		}
	}
}

void enemiesAlways() {
	int p = 0;
	int id = 0;
	float angle = 0;
	float amt = 0;
	string db = "";
	/*
	Enemies incoming
	*/
	for (x=xsMin(5, xGetDatabaseCount("enemiesIncoming")); > 0) {
		yDatabaseNext("enemiesIncoming", true);
		for(p=1; < ENEMY_PLAYER) {
			if (trUnitHasLOS(p)) {
				activateEnemy("enemiesIncoming");
				xFreeDatabaseBlock("enemiesIncoming");
				break;
			}
		}
	}
	
	if (xGetDatabaseCount(dEnemies) > 0) {
		id = yDatabaseNext("enemies", true);
		if ((id == -1) || (trUnitAlive() == false)) {
			removeEnemy();
		} else if ((kbUnitGetAnimationActionType(id) == 9) &&
			(xGetInt("enemies", xStunStatus) + xGetInt("enemies", "poisonStatus") + xGetInt("enemies", xSilenceStatus) == 0)) {
			/* if idle and out of LOS, we remove it */
			xSetInt("enemies", "missing", 1 + xGetInt("enemies", "missing"));
			if (xGetInt("enemies", "missing") >= 10) {
				for(p=1; < ENEMY_PLAYER) {
					if (trUnitHasLOS(p)) {
						xSetInt("enemies", "missing", 0);
						break;
					}
				}
				if (xGetInt("enemies", "missing") >= 10) {
					yAddToDatabase("enemiesIncoming", "enemies");
					xFreeDatabaseBlock("enemies");
				}
			}
		} else {
			stunsAndPoisons("enemies");
			trVectorSetUnitPos("pos", "enemies");
			xSetInt("enemies", "posX", trQuestVarGet("posX"));
			xSetInt("enemies", "posZ", trQuestVarGet("posZ"));
		}
	}
	
	/* ambush rooms */
	if (xGetDatabaseCount("ambushRooms") > 0) {
		yDatabaseNext("ambushRooms");
		trQuestVarSet("posX", xGetInt("ambushRooms", "posX"));
		trQuestVarSet("posZ", xGetInt("ambushRooms", "posZ"));
		for(p=1; < ENEMY_PLAYER) {
			if (zDistanceToVectorSquared("p"+p+"unit", "pos") < 100) {
				db = trStringQuestVarGet("enemyProto"+xGetInt("ambushRooms", "type"));
				trQuestVarSetFromRand("count", trQuestVarGet("stage"), 11, true);
				angle = 6.283185 / trQuestVarGet("count");
				amt = 0;
				trQuestVarSet("angle",0);
				for(x=trQuestVarGet("count"); >0) {
					trQuestVarSetFromRand("dist", 6, 18, true);
					trVectorSetFromAngle("dir", trQuestVarGet("angle"));
					trVectorScale("dir", trQuestVarGet("dist"));
					trQuestVarSet("dirX", trQuestVarGet("posX") + trQuestVarGet("dirX"));
					trQuestVarSet("dirZ", trQuestVarGet("posZ") + trQuestVarGet("dirZ"));
					amt = fModulo(360.0, trQuestVarGet("angle") * 180.0 / 3.141592 - 180.0);
					trQuestVarSet(xSpecialNext, trGetNextUnitScenarioNameNumber());
					trArmyDispatch("1,0",db,1,trQuestVarGet("dirX"),0,trQuestVarGet("dirZ"),amt,true);
					trUnitSelectClear();
					trUnitSelectByQV(xSpecialNext, true);
					trUnitConvert(ENEMY_PLAYER);
					activateEnemy(xSpecialNext);
					trQuestVarSet("angle", trQuestVarGet("angle") + angle);
				}
				trSoundPlayFN("attackwarning.wav","1",-1,"","");
				trSoundPlayFN("wild.wav","1",-1,"","");
				xFreeDatabaseBlock("ambushRooms");
				break;
			}
		}
	}
}

