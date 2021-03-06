// (c) Copyright HutongGames, LLC 2010-2011. All rights reserved.

using UnityEngine;

namespace HutongGames.PlayMaker.Actions
{
	
	
	[ActionCategory(ActionCategory.Transform)]
	[Tooltip("Moves a Game Object towards a Target. Optionally sends an event when successful. Optionally set when to update, during regular update, lateUpdate or FixedUpdate. The Target can be specified as a Game Object or a world Position. If you specify both, then the Position is used as a local offset from the Object's Position.")]
	public class MoveTowards2 : FsmStateAction
	{
		public enum UpdateType {Update,LateUpdate,FixedUpdate};
		
		[RequiredField]
		public FsmOwnerDefault gameObject;
		
		public FsmGameObject targetObject;
		
		public FsmVector3 targetPosition;
		
		public FsmBool ignoreVertical;
		
		[HasFloatSlider(0, 20)]
		public FsmFloat maxSpeed;
		
		[HasFloatSlider(0, 5)]
		public FsmFloat finishDistance;
		
		public FsmEvent finishEvent;
		
		public UpdateType updateType;
		

		public override void Reset()
		{
			gameObject = null;
			targetObject = null;
			maxSpeed = 10f;
			finishDistance = 1f;
			finishEvent = null;
			updateType = UpdateType.Update;
		}

		public override void OnUpdate()
		{
			if (updateType == UpdateType.Update)
			{
				DoMoveTowards();
			}
		}
		
		public override void OnLateUpdate()
		{
			if (updateType == UpdateType.LateUpdate)
			{
				DoMoveTowards();
			}
		}
		
		public override void OnFixedUpdate()
		{
			//if (updateType == UpdateType.FixedUpdate)
			//{
				DoMoveTowards();
			//}
		}

		void DoMoveTowards()
		{
			var go = Fsm.GetOwnerDefaultTarget(gameObject);
			if (go == null)
			{
				return;
			}
			
			var goTarget = targetObject.Value;
			if (goTarget == null && targetPosition.IsNone)
			{
				return;
			}

			Vector3 targetPos;
			if (goTarget != null)
			{
				targetPos = !targetPosition.IsNone ? 
					goTarget.transform.TransformPoint(targetPosition.Value) : 
					goTarget.transform.position;
			}
			else
			{
				targetPos = targetPosition.Value;
			}

			if (ignoreVertical.Value)
			{
				targetPos.y = go.transform.position.y;
			}
			
			go.transform.position = Vector3.MoveTowards(go.transform.position, targetPos, maxSpeed.Value * Time.deltaTime);
			
			var distance = (go.transform.position - targetPos).magnitude;
			if (distance < finishDistance.Value)
			{
				Fsm.Event(finishEvent);
				Finish();
			}
		}

	}
}