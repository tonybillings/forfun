Shader "Custom/NebulaeParticle" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_Ramp ("Ramp (RGB)", 2D) = "white" {}
		_TintColor ("Tint Color", Color) = (0.5,0.5,0.5,0.5)
	}
	SubShader {
		Tags { "RenderType"="Opaque" }
		LOD 200
		Cull Off 
		Lighting Off 
		ZWrite Off
		Blend One One
		CGPROGRAM
		#pragma surface surf Lambert

		sampler2D _MainTex;
		sampler2D _Ramp;
		half4 _TintColor;

		struct Input {
			float2 uv_MainTex;
		};

		void surf (Input IN, inout SurfaceOutput o) {
			half c = tex2D (_MainTex, IN.uv_MainTex).a;
			half2 uv = (c,c);
			o.Albedo = (tex2D(_Ramp, uv) * c) * _TintColor ;
			o.Alpha = 1;
		}
		ENDCG
	} 
	FallBack "Diffuse"
}
