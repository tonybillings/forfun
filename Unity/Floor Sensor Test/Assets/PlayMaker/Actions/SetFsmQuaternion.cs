// (c) Copyright HutongGames, LLC 2010-2013. All rights reserved.

using UnityEngine;

namespace HutongGames.PlayMaker.Actions
{
	[ActionCategory(ActionCategory.StateMachine)]
	[Tooltip("Set the value of a Quaternion Variable in another FSM.")]
	public class SetFsmQuaternion : FsmStateAction
	{
		[RequiredField]
		[Tooltip("The GameObject that owns the FSM.")]
		public FsmOwnerDefault gameObject;

		[UIHint(UIHint.FsmName)]
		[Tooltip("Optional name of FSM on Game Object")]
		public FsmString fsmName;

		[RequiredField]
		[UIHint(UIHint.FsmQuaternion)]
        [Tooltip("The name of the FSM variable.")]
		public FsmString variableName;

		[RequiredField]
        [Tooltip("Set the value of the variable.")]
		public FsmQuaternion setValue;

        [Tooltip("Repeat every frame. Useful if the value is changing.")]
        public bool everyFrame;

		GameObject goLastFrame;
		PlayMakerFSM fsm;

		public override void Reset()
		{
			gameObject = null;
			fsmName = "";
			variableName = "";
			setValue = null;
			everyFrame = false;
		}

		public override void OnEnter()
		{
			DoSetFsmQuaternion();

			if (!everyFrame)
			{
				Finish();
			}
		}

		void DoSetFsmQuaternion()
		{
			if (setValue == null)
			{
				return;
			}

			var go = Fsm.GetOwnerDefaultTarget(gameObject);
			if (go == null)
			{
				return;
			}

			if (go != goLastFrame)
			{
				goLastFrame = go;

				// only get the fsm component if go has changed

				fsm = ActionHelpers.GetGameObjectFsm(go, fsmName.Value);
			}

			if (fsm == null)
			{
                LogWarning("Could not find FSM: " + fsmName.Value);
				return;
			}

			var fsmVar = fsm.FsmVariables.GetFsmQuaternion(variableName.Value);

			if (fsmVar != null)
			{
				fsmVar.Value = setValue.Value;
			}
            else
            {
                LogWarning("Could not find variable: " + variableName.Value);
            }
		}

		public override void OnUpdate()
		{
			DoSetFsmQuaternion();
		}

	}
}