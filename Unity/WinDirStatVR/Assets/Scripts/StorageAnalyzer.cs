using System.Collections.Generic;
using System.IO;
using System.Threading;

public static class StorageAnalyzer
{
    #region Private Variables
    private static volatile Stack<Folder> _stack1;
    private static volatile Stack<Folder> _stack2;
    private static volatile Stack<string> _workerStack;
    private static volatile Folder _currentFolder;
    private static readonly object _stackLock;
    private static readonly object _folderLock;
    #endregion

    #region Constructor
    static StorageAnalyzer()
    {
        _stackLock = new object();
        _folderLock = new object();
    }
    #endregion

    #region Private Methods
    private static void ThreadWork(object state)
    {
        string folder = null;
        bool workFound = false;

        lock (_stackLock)
        {
            if (_workerStack.Count != 0)
            {
                folder = _workerStack.Pop();
                workFound = true;
            }
        }

        if (workFound)
        {
            Folder subFolder = new Folder()
            {
                Path = folder,
                FilesSize = GetFilesSize(folder)
            };

            lock (_folderLock)
            {
                _currentFolder.SubFolders.Add(subFolder);
                _stack1.Push(subFolder);
                _stack2.Push(subFolder);
            }
        }
    }

    private static long GetFilesSize(string path)
    {
        long size = 0;

        try
        {
            DirectoryInfo di = new DirectoryInfo(path);
            FileInfo[] fis = di.GetFiles();

            foreach (FileInfo fi in fis)
                size += fi.Length;
        }
        catch { }

        return size;
    }
    #endregion

    #region Public Methods
    public static Folder GetFolder_MultiThreaded(string folderPath)
    {
        Folder root = new Folder()
        {
            Path = folderPath,
            FilesSize = GetFilesSize(folderPath)
        };

        _stack1 = new Stack<Folder>();
        _stack2 = new Stack<Folder>();
        _workerStack = new Stack<string>();

        _stack1.Push(root);
        _stack2.Push(root);

        while (_stack1.Count != 0)
        {
            _currentFolder = _stack1.Pop();
            string[] subDirectories = null;

            try
            {
                subDirectories = Directory.GetDirectories(_currentFolder.Path);
            }
            catch { }

            if (subDirectories != null)
            {
                lock (_stackLock)
                {
                    foreach (string subDirectory in subDirectories)
                    {
                        _workerStack.Push(subDirectory);
                        ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadWork));
                    }
                }

                bool threadsWorking = true;
                while (threadsWorking)
                {
                    lock (_folderLock)
                    {
                        threadsWorking = _currentFolder.SubFolders.Count != subDirectories.Length;
                    }
                }

                _currentFolder.SubFolders.Sort(delegate (Folder a, Folder b) { return a.Path.CompareTo(b.Path); });
            }
        }

        while (_stack2.Count != 0)
        {
            Folder folder = _stack2.Pop();
            folder.TotalSize = folder.FilesSize;
            folder.SubFolders.ForEach(f => folder.TotalSize += f.TotalSize);
        }

        return root;
    }

    public static Folder GetFolder(string folderPath)
    {
        Folder root = new Folder()
        {
            Path = folderPath,
            FilesSize = GetFilesSize(folderPath)
        };

        Stack<Folder> stack1 = new Stack<Folder>();
        Stack<Folder> stack2 = new Stack<Folder>();

        stack1.Push(root);
        stack2.Push(root);

        while (stack1.Count != 0)
        {
            Folder currentFolder = stack1.Pop();
            string[] subDirectories = null;

            try
            {
                subDirectories = Directory.GetDirectories(currentFolder.Path);
            }
            catch { }

            if (subDirectories != null)
            {
                foreach (string subDirectory in subDirectories)
                {
                    Folder subFolder = new Folder()
                    {
                        Path = subDirectory,
                        FilesSize = GetFilesSize(subDirectory)
                    };

                    currentFolder.SubFolders.Add(subFolder);
                    stack1.Push(subFolder);
                    stack2.Push(subFolder);
                }
            }
        }

        while (stack2.Count != 0)
        {
            Folder folder = stack2.Pop();
            folder.TotalSize = folder.FilesSize;
            folder.SubFolders.ForEach(f => folder.TotalSize += f.TotalSize);
        }

        return root;
    }
    #endregion
}
