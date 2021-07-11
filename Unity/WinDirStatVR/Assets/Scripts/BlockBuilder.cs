using System.Collections.Generic;
using UnityEngine;

public class BlockBuilder : MonoBehaviour 
{
    #region Public Variables
    public Material[] Materials;
    public GameObject BlockPrefab;
    public float GraphHeight;
    public float GraphLength;
    #endregion

    #region Private Methods
    private sbyte GetRandomNumber(sbyte upperBound, sbyte exceptionNumber = -1)
    {
        sbyte result;
        while ((result = (sbyte)Random.Range(0, upperBound)) == exceptionNumber) { }
        return result;
    }

    private string GetSizeText(Folder folder)
    {
        return string.Concat(ConvertSizeToText(folder.FilesSize), " / ", ConvertSizeToText(folder.TotalSize));
    }

    private string ConvertSizeToText(float size)
    {
        float temp;

        if ((temp = size / 1073741824f) > 1)
        {
            return string.Concat(temp.ToString("N3"), " GB");
        }
        else if ((temp = size / 1048576f) > 1)
        {
            return string.Concat(temp.ToString("N3"), " MB");
        }
        else if ((temp = size / 1024) > 1)
        {
            return string.Concat(temp.ToString("N3"), " KB");
        }
        else
        {
            return string.Concat(temp.ToString("N0"), " B");
        }
    }
    #endregion

    #region Public Methods
    public void Build(Folder rootFolder)
    {
        Random.seed = (int)System.DateTime.Now.Ticks;
        GameObject rootGo = Instantiate(BlockPrefab);

        rootGo.name = string.Concat(rootFolder.Path, "\r\n", GetSizeText(rootFolder));

        rootGo.transform.parent = this.transform;
        rootGo.transform.localScale = new Vector3(GraphLength, Mathf.Max(GraphHeight * (rootFolder.FilesSize / (float)rootFolder.TotalSize), 0.1f), GraphLength / 2f);
        rootGo.transform.position += new Vector3(0, rootGo.transform.localScale.y / 2f, 0);

        sbyte randomIndex = GetRandomNumber((sbyte)Materials.Length);
        rootGo.GetComponent<Renderer>().sharedMaterial = Materials[randomIndex];

        FolderMetaData rootFmd = rootGo.GetComponent<FolderMetaData>();
        rootFmd.FolderInfo = rootFolder;
        rootFmd.MaterialIndex = randomIndex;

        Stack<Folder> folderStack = new Stack<Folder>();
        Stack<GameObject> gameObjectStack = new Stack<GameObject>();
        Stack<sbyte> randomIndexStack = new Stack<sbyte>();

        folderStack.Push(rootFolder);
        gameObjectStack.Push(rootGo);
        randomIndexStack.Push(randomIndex);

        while (folderStack.Count != 0)
        {
            Folder parentFolder = folderStack.Pop();
            GameObject parentGo = gameObjectStack.Pop();
            sbyte parentRandomIndex = randomIndexStack.Pop();

            int i = 0;

            float x, z;
            Vector3 pgoSize = parentGo.transform.localScale;
            Vector3 pgoHalfSize = parentGo.transform.localScale / 2f;

            if (pgoSize.x > pgoSize.z)
            {
                x = pgoSize.x / parentFolder.SubFolders.Count;
                z = pgoSize.z;
            }
            else
            {
                z = pgoSize.z / parentFolder.SubFolders.Count;
                x = pgoSize.x;
            }

            foreach (Folder subFolder in parentFolder.SubFolders)
            {
                GameObject go = Instantiate(BlockPrefab);

                go.name = string.Concat(subFolder.Path, "\r\n", GetSizeText(subFolder));

                go.transform.parent = this.transform;
                go.transform.localScale = new Vector3(x, Mathf.Max(GraphHeight * (subFolder.FilesSize / (float)rootFolder.TotalSize), 0.1f), z);
                go.transform.position = parentGo.transform.position;

                Vector3 goSize = go.transform.localScale;
                Vector3 goHalfSize = go.transform.localScale / 2f;
                float verticalPos = pgoHalfSize.y + goHalfSize.y;

                if (pgoSize.x > pgoSize.z)
                {
                    go.transform.position += new Vector3(-pgoHalfSize.x + goHalfSize.x, 0, 0);
                    go.transform.position += new Vector3(x * i, verticalPos, 0);
                }
                else
                {
                    go.transform.position += new Vector3(0, 0, -pgoHalfSize.z + goHalfSize.z);
                    go.transform.position += new Vector3(0, verticalPos, z * i);
                }

                go.transform.localScale = new Vector3(goSize.x * 0.9f, goSize.y, goSize.z * 0.9f);

                sbyte childRandomIndex = GetRandomNumber((sbyte)Materials.Length, parentRandomIndex);
                go.GetComponent<Renderer>().sharedMaterial = Materials[childRandomIndex];

                FolderMetaData fmd = go.GetComponent<FolderMetaData>();
                fmd.FolderInfo = subFolder;
                fmd.MaterialIndex = childRandomIndex;

                folderStack.Push(subFolder);
                gameObjectStack.Push(go);
                randomIndexStack.Push(childRandomIndex);

                i++;
            }
        }
    }
    #endregion
}

