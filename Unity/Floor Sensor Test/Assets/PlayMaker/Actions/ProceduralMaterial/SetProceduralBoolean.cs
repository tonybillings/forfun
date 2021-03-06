// (c) Copyright HutongGames, LLC 2010-2013. All rights reserved.

using UnityEngine;

namespace HutongGames.PlayMaker.Actions
{
	[ActionCategory("Substance")]
	[Tooltip("Set a named bool property in a Substance material. NOTE: Use Rebuild Textures after setting Substance properties.")]
	public class SetProceduralBoolean : FsmStateAction
	{
		[RequiredField]
		public FsmMaterial substanceMaterial;
		[RequiredField]
		public FsmString boolProperty;
		[RequiredField]
		public FsmBool boolValue;
		[Tooltip("NOTE: Updating procedural materials every frame can be very slow!")]
		public bool everyFrame;

		public override void Reset()
		{
			substanceMaterial = null;
			boolProperty = "";
			boolValue = false;
			everyFrame = false;
		}

		public override void OnEnter()
		{
			DoSetProceduralFloat();

			if (!everyFrame)
			{
				Finish();
			}
		}

		public override void OnUpdate()
		{
			DoSetProceduralFloat();
		}

		void DoSetProceduralFloat()
        {
#if !(UNITY_IPHONE || UNITY_ANDROID || UNITY_NACL || UNITY_FLASH || UNITY_PS3 || UNITY_BLACKBERRY || UNITY_METRO || UNITY_WP8 || UNITY_WIIU || UNITY_PSM || UNITY_WEBGL)

            var substance = substanceMaterial.Value as ProceduralMaterial;

			if (substance == null)
			{
				LogError("Not a substance material!");
				return;
			}

			substance.SetProceduralBoolean(boolProperty.Value, boolValue.Value);

#endif
        }
	}
}