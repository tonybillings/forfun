using System;
using System.Collections.Generic;

public class Folder
{
    public string Path;
    public long TotalSize;
    public long FilesSize;
    public List<Folder> SubFolders;

    public Folder()
    {
        TotalSize = FilesSize = -1;
        SubFolders = new List<Folder>();
    }
}
