using UnityEngine;
using System.Collections;

public class FilesViewManager : MonoBehaviour 
{
    #region Public Variables
    public Material[] Materials;
    public Renderer FloorRenderer;
    #endregion

    public void SetRoomMaterial(int materialIndex)
    {
        FloorRenderer.sharedMaterial = Materials[materialIndex];
    }
}
