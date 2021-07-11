using UnityEngine;

public class MainScript : MonoBehaviour 
{
    public string FolderPath;
    public BlockBuilder BlockBuilderScript;

	private void Start() 
	{
        Folder root = StorageAnalyzer.GetFolder_MultiThreaded(FolderPath);
        BlockBuilderScript.Build(root);
    }
}
