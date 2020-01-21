class Izh18Recoil: RecoilBase
{
	override void Init()
	{
		vector point_1;
		vector point_2;
		vector point_3;
		vector point_4;
		point_1[0] = Math.RandomFloatInclusive(0.25,0.5); point_1[1] = 1.75; point_1[2] = 0;
		point_2[0] = Math.RandomFloatInclusive(-0.25,0.25); point_2[1] = 2; point_2[2] = 0;
		point_3[0] = Math.RandomFloatInclusive(-0.25,-0.5); point_3[1] = 1.5; point_3[2] = 0;
		point_4[0] = Math.RandomFloatInclusive(-0.25,-0.1); point_4[1] = 0.75; point_4[2] = 0;
		m_HandsCurvePoints.Insert(point_1);//forms a 2 dimensional spline(z is ignored)
		m_HandsCurvePoints.Insert(point_2);
		m_HandsCurvePoints.Insert(point_3);
		m_HandsCurvePoints.Insert(point_4);
		m_HandsCurvePoints.Insert("0 0 0");
		m_HandsOffsetRelativeTime = 0.125;
		
		m_MouseOffsetRangeMin = 80;//in degrees min
		m_MouseOffsetRangeMax = 100;//in degrees max
		m_MouseOffsetDistance = 1.4;//how far should the mouse travel
		m_MouseOffsetRelativeTime = 0.0625;//[0..1] a time it takes to move the mouse the required distance relative to the reload time of the weapon(firing mode)
	
		m_CamOffsetDistance = 0.02;
		m_CamOffsetRelativeTime = 0.125;
	}
}