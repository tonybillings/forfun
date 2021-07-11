using System;
using System.Windows.Forms;
using System.Linq;

namespace TIDE.Code.Parsing
{
    public class CodeTree<T> where T : CodeTreeNode, ICodeTreeNode, new()
    {
        #region Private Variables
        private T _rootNode;
        private int _lineCount;
        private int _nodeCount;
        private int _groupCount;
        private int _coreCount;
        #endregion

        #region Properties
        public T RootNode { get { return _rootNode; } }
        public int LineCount { get { return _lineCount; } }
        public int NodeCount { get { return _nodeCount; } }
        public int GroupCount { get { return _groupCount; } }
        public int CoreCount { get { return _coreCount; } }
        #endregion

        #region Constructors
        public CodeTree() {}

        public CodeTree(string code)
        {
            Build(code);
        }
        #endregion

        #region Public Methods
        public void Build(string code)
        {
            if (String.IsNullOrEmpty(code)) return;

            _rootNode = new T();
            _rootNode.Append(code);
            _rootNode.Construct();
            _rootNode.Parse();
            _rootNode.Rename(new CodeTreeNode.NodeName());

            _lineCount = code.Replace("\r\n", "\n").Split("\n".ToCharArray()).Length;
            _nodeCount = GetNodeCount(_rootNode);
            _groupCount = GetNodeCount(_rootNode, ScopeType.Group);
            _coreCount = GetNodeCount(_rootNode, ScopeType.Core);
        }

        public void Clear()
        {
            _rootNode = default(T);
            _lineCount = 0;
            _nodeCount = 0;
        }
        #endregion

        #region Private Methods
        private int GetNodeCount(T parent)
        {
            int count = parent.Children.Count;

            foreach (T child in parent.Children)
                count += GetNodeCount(child);

            return count;
        }

        private int GetNodeCount(T parent, ScopeType scopeType)
        {
            int count = parent.Children.Where(n => n.ScopeType == scopeType).Count();

            foreach (T child in parent.Children)
                count += GetNodeCount(child, scopeType);

            return count;
        }
        #endregion
    }
}
