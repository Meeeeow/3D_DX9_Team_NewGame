#pragma once
#ifndef __CLIENT_CONST_H__
#define __CLIENT_CONST_H__

namespace Client
{
	//컴포넌트
	static const wchar_t* pTagComTransform = L"ComTransform";
	static const wchar_t* pTagComRenderer = L"ComRenderer";
	static const wchar_t* pTagComVIBuffer = L"ComVIBuffer";
	static const wchar_t* pTagComTexture = L"ComTexture";
	static const wchar_t* pTagComFrustum = L"ComFrustum";
	static const wchar_t* pTagComCollider = L"ComCollider";

	//컴포넌트 프로토타입
	static const wchar_t* pTagPrototypeTransform = L"PrototypeTransform";
	static const wchar_t* pTagPrototypeRenderer = L"PrototypeRenderer";
	static const wchar_t* pTagPrototypeVIBufferRect = L"PrototypeVIBufferRect";
	static const wchar_t* pTagPrototypeFrustum = L"PrototypeFrustum";
	static const wchar_t* pTagPrototypeVIBufferTerrain = L"PrototypeVIBufferTerrain";
	static const wchar_t* pTagPrototypeVIBufferCube = L"PrototypeVIBufferCube";
	static const wchar_t* pTagPrototypeSubject = L"PrototypeSubject";
	static const wchar_t* pTagPrototypeBoxCollider = L"PrototypeBoxCollider";
	static const wchar_t* pTagPrototypeVIBufferRectColor = L"PrototypeVIBufferRectColor";
	static const wchar_t* pTagPrototypeVIBufferThorn = L"PrototypeVIBufferThorn";
	

	//텍스쳐 프로토타입
	static const wchar_t* pTagPrototypeTextureDmgFont = L"PrototypeTextureDmgFont";
	static const wchar_t* pTagPrototypeTextureBrownCube = L"PrototypeTextureBrownCube";
	static const wchar_t* pTagPrototypeTextureRainDrop = L"PrototypeTextureRainDrop";
	static const wchar_t* pTagPrototypeTextureBossSkeletonStageCube = L"PrototypeTextureBossSkeletonStageCube";
	static const wchar_t* pTagPrototypeTextureCrosshair = L"PrototypeTextureCrosshair";
	static const wchar_t* pTagPrototypeTextureTrumpClover = L"PrototypeTextureTrumpClover";
	static const wchar_t* pTagPrototypeTextureTrumpHeart = L"PrototypeTextureTrumpHeart";
	static const wchar_t* pTagPrototypeTextureTrumpDiamond = L"PrototypeTextureTrumpDiamond";
	static const wchar_t* pTagPrototypeTextureTrumpSpade = L"PrototypeTextureTrumpSpade";
	static const wchar_t* pTagPrototypeTextureBlindEffect = L"ProtottypeTextureBlindEffect";
	static const wchar_t* pTagPrototpyeTextureBossClown = L"PrototypeTextureBossClown";
	static const wchar_t* pTagPrototypeTextureDagger = L"PrototypeTextureDagger";
	static const wchar_t* pTagPrototypeTextureBossSkeletonStageCubeAttack = L"PrototypeTextureBossSkeletonStageCubeAttack";
	static const wchar_t* pTagPrototypeTextureBossSkeletonStageCircle = L"PrototypeTextureBossSkeletonStageCircle";
	static const wchar_t* pTagPrototypeTextureBossSkeletonThorn = L"PrototypeTextureBossSkeletonThorn";
	static const wchar_t* pTagPrototypeTextureBossSkeleton = L"PrototypeTextureBossSkeleton";
	static const wchar_t* pTagPrototypeTextureWall = L"PrototypeTextureWall";
	static const wchar_t* pTagPrototypeTextureBlack = L"PrototypeTextureBlack";
	static const wchar_t* pTagPrototypeTextureLoading = L"PrototypeTextureLoading";
	static const wchar_t* pTagPrototypeTexturePlayerInfo = L"PrototypeTexturePlayerInfo";
	static const wchar_t* pTagPrototypeTextureMeteor = L"PrototypeTextureMeteor";
	static const wchar_t* pTagPrototypeTextureLockOn = L"PrototypeTextureLockOn";
	static const wchar_t* pTagPrototypeTexturePoisonCube = L"PrototypeTexturePoisonCube";
	static const wchar_t* pTagPrototypeTexturePlayerShield = L"PrototypeTexturePlayerShield";
	static const wchar_t* pTagPrototypeTextureBloodScreen = L"PrototypeTextureBloodScreen";
	static const wchar_t* pTagPrototypeTextureMagicMissile = L"PrototypeTextureMagicMissile";
	static const wchar_t* pTagPrototypeTextureTempleCube = L"PrototypeTextureTempleCube";
	static const wchar_t* pTagPrototypeTextureTempleBrokenCube = L"PrototypeTextureTemplBrokenCube";
	static const wchar_t* pTagPrototypeTextureFireCracker = L"PrototypeTextureFireCracker";
	static const wchar_t* pTagPrototypeTextureConfetti = L"PrototypeTextureConfetti";
	static const wchar_t* pTagPrototypeTextureBlackAlpha = L"PrototypeTextureBalck80Alpha";
	static const wchar_t* pTagPrototypeTextureTipObject = L"PrototypeTextureTipObject";
	static const wchar_t* pTagPrototypeTextureCircusTip = L"PrototypeTextureCircusTip";


	//게임오브젝트 프로토타입
	static const wchar_t* pTagPrototypeDmgFont = L"PrototypeDmgFont";
	static const wchar_t* pTagPrototypeDestroyableCube = L"PrototypeDestroyableCube";
	static const wchar_t* pTagPrototypeAttack = L"PrototypeAttack";
	static const wchar_t* pTagPrototypeMonsterNormalMelee = L"PrototypeMonsterNormalMelee";
	static const wchar_t* pTagPrototypeMonsterNormalRange = L"PrototypeMonsterNormalRange";
	static const wchar_t* pTagPrototypeMagicMissile = L"PrototypeMagicMissile";
	static const wchar_t* pTagPrototypeRainDrop = L"PrototypeRainDrop";
	static const wchar_t* pTagPrototypeRain = L"PrototypeRain";
	static const wchar_t* pTagPrototypeBossSkeletonStageCube = L"PrototypeBossSkeletonStageCube";
	static const wchar_t* pTagPrototypeCrosshair = L"PrototypeCrosshair";
	static const wchar_t* pTagPrototypeBossSkeletonStageCubeObserver = L"PrototypeBossSkeletonStageCubeObserver";
	static const wchar_t* pTagPrototypeBossSkeletonStageCubeAttack = L"PrototypeBossSkeletonStageCubeAttack";
	static const wchar_t* pTagPrototypeBossSkeletonStageCircle = L"PrototypeBossSkeletonStageCircle";
	static const wchar_t* pTagPrototypeBossSkeletonThorn = L"PrototypeBossSkeletonThorn";
	static const wchar_t* pTagPrototypeBossSkeleton = L"PrototypeBossSkeleton";
	static const wchar_t* pTagPrototypeWallLook = L"PrototypeWallLook";
	static const wchar_t* pTagPrototypeWallSide = L"PrototypeWallSide";
	static const wchar_t* pTagPrototypeCircusBossClown = L"PrototypeBossClown";
	static const wchar_t* pTagPrototypeCircusCubeBroken = L"PrototypeCubeLegoBroken";
	static const wchar_t* pTagPrototypeCircusCubeLegoRed = L"PrototypeCubeLegoRed";
	static const wchar_t* pTagPrototypeCircusCubeLegoBlue = L"PrototypeCubeLegoBlue";
	static const wchar_t* pTagPrototypeCircusCubeLegoGray = L"PrototypeCubeLegoGray";
	static const wchar_t* pTagPrototypeCircusWallLegoRedL = L"PrototypeWallLegoRedL";
	static const wchar_t* pTagPrototypeCircusWallLegoRedR = L"PrototypeWallLegoRedR";
	static const wchar_t* pTagPrototypeCircusWallLegoRedF = L"PrototypeWallLegoRedF";
	static const wchar_t* pTagPrototypeCircusWallLegoRedB = L"PrototypeWallLegoRedB";
	static const wchar_t* pTagPrototypeCircusWallLegoBlueL = L"PrototypeWallLegoBlueL";
	static const wchar_t* pTagPrototypeCircusWallLegoBlueR = L"PrototypeWallLegoBlueR";
	static const wchar_t* pTagPrototypeCircusWallLegoBlueF = L"PrototypeWallLegoBlueF";
	static const wchar_t* pTagPrototypeCircusWallLegoBlueB = L"PrototypeWallLegoBlueB";
	static const wchar_t* pTagPrototypeCircusWallLegoGrayL = L"PrototypeWallLegoGrayL";
	static const wchar_t* pTagPrototypeCircusWallLegoGrayR = L"PrototypeWallLegoGrayR";
	static const wchar_t* pTagPrototypeCircusWallLegoGrayF = L"PrototypeWallLegoGrayF";
	static const wchar_t* pTagPrototypeCircusWallLegoGrayB = L"PrototypeWallLegoGrayB";
	static const wchar_t* pTagPrototypeBossSkeletonStageCircleAttack = L"PrototypeBossSkeletonStageCircleAttack";
	static const wchar_t* pTagPrototypeCircusCardDiamond = L"PrototypeCardDiamond";
	static const wchar_t* pTagPrototypeCircusCardHeart = L"PrototypeCardHeart";
	static const wchar_t* pTagPrototypeCircusCardSpade = L"PrototypeCardSpade";
	static const wchar_t* pTagPrototypeCircusCardClover = L"PrototypeCardClover";
	static const wchar_t* pTagPrototypeEffectBlind = L"PrototypeEffectBlind";
	static const wchar_t* pTagPrototypeTagGauge = L"PrototypeTagGauge";
	static const wchar_t* pTagPrototypeObserverTagGauge = L"PrototypeObserverTagGauge";
	static const wchar_t* pTagPrototypeNonControl = L"PrototypeNonControl";
	static const wchar_t* pTagPrototypeObserverNonControl = L"PrototypeObserverNonControl";
	static const wchar_t* pTagPrototypePortal = L"PrototypePortal";
	static const wchar_t* pTagPrototypeDagger = L"PrototypeDagger";
	static const wchar_t* pTagPrototypeMeteor = L"PrototypeMeteor";
	static const wchar_t* pTagPrototypeLockOn = L"PrototypeLockOn";
	static const wchar_t* pTagPrototypePoisonCube = L"PrototypePoisonCube";
	static const wchar_t* pTagPrototypePoisonCubeParticle = L"PrototypePoisonCubeParticle";
	static const wchar_t* pTagPrototypePoisonCubeAttack = L"PrototypePoisonCubeAttack";
	static const wchar_t* pTagPrototypePoisonCubeHit = L"PrototypePoisonCubeHit";
	static const wchar_t* pTagPrototypePlayerShield = L"PrototypePlayerShield";
	static const wchar_t* pTagPrototypeBloodScreen = L"PrototypeBloodScreen";
	static const wchar_t* pTagPrototypeMagicMissileParticle = L"PrototypeMagicMissileParticle";
	static const wchar_t* pTagPrototypeRunWet = L"PrototypeRunWet";
	static const wchar_t* pTagPrototypeRunWetParticle = L"PrototypeRunWetParticle";
	static const wchar_t* pTagPrototypeFireCracker = L"PrototypeFireCracker";
	static const wchar_t* pTagPrototypeConfetti = L"PrototypeConfetti";
	static const wchar_t* pTagPrototypeCaveBloc = L"PrototypeCaveBloc";
	static const wchar_t* pTagPrototypeCaveDoorDestroyable = L"PrototypeCaveDoorDestroyable";
	static const wchar_t* pTagPrototypeDialog = L"PrototypeDialog";
	static const wchar_t* pTagPrototypeTriggerCollider = L"PrototypeTriggerCollider";
	static const wchar_t* pTagPrototypeWaterCannon = L"PrototypeWaterCannon";
	static const wchar_t* pTagPrototypeBlackScreen = L"PrototypeBlackScreen";


	//게임오브젝트 레이어
	static const wchar_t* pTagLayerWall = L"LayerWall";
	static const wchar_t* pTagLayerDmgFont = L"LayerDmgFont";
	static const wchar_t* pTagLayerDestroyableCube = L"LayerDestroyableCube";
	static const wchar_t* pTagLayerPlayer = L"LayerPlayer";
	static const wchar_t* pTagLayerMonster = L"LayerMonster";
	static const wchar_t* pTagLayerPlayerAttack = L"LayerPlayerAttack";
	static const wchar_t* pTagLayerMonsterAttack = L"LayerMonsterAttack";
	static const wchar_t* pTagLayerRainDrop = L"LayerRainDrop";
	static const wchar_t* pTagLayerRain = L"LayerRain";
	static const wchar_t* pTagLayerEnvironment = L"LayerEnvironment";
	static const wchar_t* pTagLayerBossSkeletonCube = L"LayerBossSkeletonCube";
	static const wchar_t* pTagLayerCrosshair = L"LayerCrosshair";
	static const wchar_t* pTagLayerObserver = L"LayerObserver";
	static const wchar_t* pTagLayerNonCollision = L"LayerNonCollision";
	static const wchar_t* pTagLayerTileCube = L"LayerTileCube";
	static const wchar_t* pTagLayerCircusCube = L"LayerCircusCube";
	static const wchar_t* pTagLayerCircusWall = L"LayerCircusWall";
	static const wchar_t* pTagLayerCircusEffect = L"LayerCircusEffect";
	static const wchar_t* pTagLayerUI = L"LayerUI";
	static const wchar_t* pTagLayerCircusBrokenCube = L"LayerTileBrokenCube";
	static const wchar_t* pTagLayerLockOn = L"LayerLockOn";
	static const wchar_t* pTagLayerPortal = L"LayerPortal";


	//씬 이름
	static const wchar_t* pTagBossSkeletonSceneName = L"BossSkeletonStage";
	static const wchar_t* pTagBossClownScnenName = L"Circus";
	static const wchar_t* pTagBossNecromencerSceneName = TEXT("Necromencer");

}


#endif // !__CLIENT_DEFINES_H__
