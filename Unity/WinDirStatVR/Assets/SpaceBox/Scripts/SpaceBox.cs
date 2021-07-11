using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;


[System.Serializable]
public class StarFieldSettings
{
	public bool enabled = true;
	public float spread = 20;
	public float distance = 1000;
	public int count = 1000;
	public float size = 100;
	public float brightness = 0.2f;
	public Material material;
	public float horizonWeight;
}


public class SpaceBox : MonoBehaviour
{
	public int size = 1024;
	public Material skyBoxMaterial;
	public int seed;
	public StarFieldSettings[] starFields;
	public StarFieldSettings[] nebulae;
	
	
	
	
	void Start() {
		Random.seed = seed;
		var destroyList = new List<GameObject>();
		
		foreach(var s in starFields) {
			if(s.enabled) {
				var g = QuadField(s.distance, s.count, s.size, s.spread, true, s.horizonWeight);
				g.GetComponent<Renderer>().material = s.material;
				g.GetComponent<Renderer>().material.SetColor("_TintColor", Color.white * s.brightness);
				destroyList.Add(g);
			}
		}
		
		foreach(var s in nebulae) {
			if(s.enabled) {
				var g = QuadField(s.distance, s.count, s.size, s.spread, false, s.horizonWeight);
				g.GetComponent<Renderer>().material = s.material;
				g.GetComponent<Renderer>().material.SetColor("_TintColor", Color.white * s.brightness);
				destroyList.Add(g);
			}
		}
		
		var cube = new Cubemap(size, TextureFormat.ARGB32, false);
		var lights = new Dictionary<Light, bool>();
		foreach(Light i in GameObject.FindSceneObjectsOfType(typeof(Light))) {
			lights[i] = i.enabled;
			i.enabled = false;
		}
		var cam = new GameObject("SpaceCam", typeof(Camera)).GetComponent<Camera>();
		var alc = RenderSettings.ambientLight;
		var fog = RenderSettings.fog;
		RenderSettings.fog = false;
		RenderSettings.ambientLight = Color.white;
		cam.clearFlags = CameraClearFlags.SolidColor;
		cam.backgroundColor = Color.black;
		cam.cullingMask = 1 << 13;
		cam.RenderToCubemap(cube);
		Destroy(cam.gameObject);
		skyBoxMaterial.SetTexture("_Tex", cube);
		RenderSettings.skybox = skyBoxMaterial;
		RenderSettings.ambientLight = alc;
		RenderSettings.fog = fog;
		foreach(var i in lights) {
			i.Key.enabled = i.Value;	
		}
		foreach(var g in destroyList) {
			Destroy(g);	
		}
		
	}
	
	
	
	GameObject QuadField(float distance, int count, float size, float spread, bool sphere, float horizonWeight) {
		var s = new GameObject();
		s.layer =  13;
		var vertices = new List<Vector3>();
		var triangles = new List<int>();
		var uvs = new List<Vector2>();
		
		var pos = Random.onUnitSphere * distance;
		
		for(var i=0; i<count; i++) {
			//s.transform.localRotation = Quaternion.identity;
			
			pos.x += Mathf.Lerp(-spread,spread, Noise.GetNoise(pos.x, pos.y, pos.z)+Random.Range(-0.1f, 0.1f));
			pos.y += Mathf.Lerp(-spread,spread, Noise.GetNoise(pos.x, pos.y, pos.z)+Random.Range(-0.1f, 0.1f));
			pos.z += Mathf.Lerp(-spread,spread, Noise.GetNoise(pos.x, pos.y, pos.z)+Random.Range(-0.1f, 0.1f));
			
			pos.y *= horizonWeight;
			
			if(sphere) {
				pos = pos.normalized * distance;
			}
			
			s.transform.position = pos;
			s.transform.LookAt(Vector3.zero, Random.onUnitSphere);
			var A = s.transform.position + ((s.transform.right - s.transform.up) * size);
			var B = s.transform.position + ((s.transform.right + s.transform.up) * size);
			var C = s.transform.position - ((s.transform.right + s.transform.up) * size);
			var D = s.transform.position - ((s.transform.right - s.transform.up) * size);
			vertices.Add(A);
			vertices.Add(B);
			vertices.Add(C);
			vertices.Add(D);
			uvs.Add(new Vector2(0,0));
			uvs.Add(new Vector2(0,1));
			
			uvs.Add(new Vector2(1,0));
			uvs.Add(new Vector2(1,1));
			var b = i*4;
			
			triangles.Add(b+0);
			triangles.Add(b+1);
			triangles.Add(b+2);
			triangles.Add(b+3);
			triangles.Add(b+2);
			triangles.Add(b+1);			
		}
		var mesh = new Mesh();
		mesh.vertices = vertices.ToArray();
		mesh.uv = uvs.ToArray();
		mesh.triangles = triangles.ToArray();
		mesh.RecalculateBounds();
		mesh.RecalculateNormals();
		s.AddComponent<MeshFilter>().mesh = mesh;
		s.AddComponent<MeshRenderer>();
		s.transform.position = Vector3.zero;
		s.transform.rotation = Quaternion.identity;
		return s;
	}
	
	
}
