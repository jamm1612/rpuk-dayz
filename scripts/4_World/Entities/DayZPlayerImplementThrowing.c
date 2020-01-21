class DayZPlayerImplementThrowing
{
	void DayZPlayerImplementThrowing(DayZPlayer pPlayer)
	{
		m_Player = pPlayer;
		m_bThrowingModeEnabled = false;
		
		ResetState();
	}
	
	void HandleThrowing(HumanInputController pHic, HumanCommandWeapons pHcw, EntityAI pEntityInHands, float pDt)
	{
		if( !pEntityInHands && !m_bThrowingAnimationPlaying )
		{
			if( m_bThrowingModeEnabled )
			{
				m_bThrowingModeEnabled = false;
				pHcw.SetThrowingMode(false);
			}
			
			return;
		}
		
		//! current state
		m_bThrowingModeEnabled = pHcw.IsThrowingMode();
		
		//! handle mode change
		if( pHic.IsThrowingModeChange() && CanChangeThrowingStance(pHic) )
		{
			ResetState();
			
			pHcw.SetActionProgressParams(0, 0);
			pHcw.SetThrowingMode(!m_bThrowingModeEnabled);
		}
		
		//! handle action
		if( m_bThrowingModeEnabled )
		{
			//! cancel throwing in case of raising hands
			if( pHic.IsWeaponRaised() )
			{
				m_bThrowingModeEnabled = false;
				ResetState();
			
				pHcw.SetActionProgressParams(0, 0);
				pHcw.SetThrowingMode(false);
				
				return;
			}
			
			//! check event for throw
			if( pHcw.WasItemLeaveHandsEvent() )
			{
				float lr = pHcw.GetBaseAimingAngleLR();
				float ud = pHcw.GetBaseAimingAngleUD();
				vector aimOrientation = m_Player.GetOrientation();
				aimOrientation[0] = aimOrientation[0] + lr;
				
				//add 5 deg 
				aimOrientation[1] = aimOrientation[1] + ud + 5;


				m_Player.GetHumanInventory().ThrowEntity(pEntityInHands, aimOrientation.AnglesToVector(), c_fThrowingForceMin + m_fThrowingForce01 * (c_fThrowingForceMax - c_fThrowingForceMin));
				return;
			}

			//! handle throw force
			if( !m_bThrowingAnimationPlaying )
			{
				if( pHic.IsUseButton() )
				{
					if( !m_bThrowingInProgress )
						m_bThrowingInProgress = true;
					
					m_fThrowingForce01 += pDt * c_fThrowingForceCoef;
					if( m_fThrowingForce01 > 1.0 )
						m_fThrowingForce01 = 1.0;
					
					pHcw.SetActionProgressParams(m_fThrowingForce01, 0);
				}
				else
				{
					if( m_bThrowingInProgress )
					{
						m_bThrowingInProgress = false;
						
						int throwType = 1;
						
						HumanItemBehaviorCfg itemCfg = m_Player.GetItemAccessor().GetItemInHandsBehaviourCfg();
						itemCfg = m_Player.GetItemAccessor().GetItemInHandsBehaviourCfg();
						if( itemCfg )
						{
							switch( itemCfg.m_iType )
							{
							case ItemBehaviorType.TWOHANDED:
								throwType = 2;
								break;
							case ItemBehaviorType.FIREARMS:
								throwType = 3;
							}
						}
						
						pHcw.ThrowItem(throwType);
						m_bThrowingAnimationPlaying = true;
					}
				}
			}
		}
		else
		{
			ResetState();
		}
	}
	
	void ResetState()
	{
		m_fThrowingForce01 = 0;
		m_bThrowingInProgress = false;
		m_bThrowingAnimationPlaying = false;
	}
	
	bool IsThrowingModeEnabled()
	{
		return m_bThrowingModeEnabled;
	}
	
	//! Throwing wind-up only (button hold)
	bool IsThrowingInProgress()
	{
		return m_bThrowingInProgress;
	}
	
	//! Throwing animation after button release
	bool IsThrowingAnimationPlaying()
	{
		return m_bThrowingAnimationPlaying;
	}
	
	bool CanChangeThrowingStance(HumanInputController pHic)
	{
		// don't change mode in raise
		if( pHic.IsWeaponRaised() )
			return false;
			
		// check if it's not a heavy item
		HumanItemBehaviorCfg itemCfg = m_Player.GetItemAccessor().GetItemInHandsBehaviourCfg();
		if( itemCfg && itemCfg.m_iType == ItemBehaviorType.HEAVY )
			return false;
		
/*		HumanMovementState movementState = new HumanMovementState();
		m_Player.GetMovementState(movementState);
		if( movementState.IsInProne() )
			return false;*/
		
		PlayerBase playerPB = PlayerBase.Cast(m_Player);
		if( playerPB && playerPB.GetEmoteManager().IsEmotePlaying() )
			return false;
		
		if( playerPB && playerPB.GetActionManager().GetRunningAction() != NULL )
			return false;
		
		if( playerPB && playerPB.IsRestrained() )
			return false;
		
		return true;
	}
	
	private DayZPlayer m_Player;
	private bool m_bThrowingModeEnabled;
	private bool m_bThrowingInProgress;
	private bool m_bThrowingAnimationPlaying;
	private float m_fThrowingForce01;
	
	private const float c_fThrowingForceMin = 20.0;
	private const float c_fThrowingForceMax = 90.0;
	private const float c_fThrowingForceCoef = 1.0;
}