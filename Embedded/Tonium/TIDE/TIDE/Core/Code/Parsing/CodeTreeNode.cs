using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TIDE.Code.Parsing
{
    #region Enums
    public enum ScopeType { NotSet, Root, Group, Core }
    #endregion

    public class CodeTreeNode
    {
        #region Nested Types
        public class NodeName
        {
            private int _groupNumber;
            private int _coreNumber;

            public void AddGroup()
            {
                _groupNumber++;
                _coreNumber = 0;
            }

            public void AddCore()
            {
                _coreNumber++;
            }

            public string GetRootName()
            {
                return "0";
            }

            public string GetGroupName()
            {
                return _groupNumber.ToString();
            }

            public string GetCoreName()
            {
                return String.Join("_", _groupNumber, _coreNumber);
            }
        }
        #endregion

        #region Protected Variables
        protected string _code;
        protected string _rawCode;
        protected StringBuilder _rawCodeStringBuilder;
        protected int _startingLineNumber;
        #endregion

        #region Properties
        public string Name { get; set; }
        public List<CodeTreeNode> Children { get; set; }
        public ScopeType ScopeType { get; set; }
        public string RawCode { get { return _rawCode; } }
        #endregion

        #region Constructors
        public CodeTreeNode()
        {
            _rawCodeStringBuilder = new StringBuilder();
            _startingLineNumber = 1;
            Name = String.Empty;
            Children = new List<CodeTreeNode>();
            ScopeType = ScopeType.Root;
        }
        #endregion

        #region Public Methods
        public void Append(string code)
        {
            _rawCodeStringBuilder.Append(code);
        }

        public void Append(char c)
        {
            _rawCodeStringBuilder.Append(c);
        }

        public void Rename(CodeTreeNode.NodeName name)
        {
            switch (ScopeType)
            {
                case ScopeType.NotSet:
                case ScopeType.Root:
                    Name = name.GetRootName();
                    Children.Where(n => n.ScopeType == ScopeType.Core).ToList().ForEach(n => n.Rename(name));
                    Children.Where(n => n.ScopeType == ScopeType.Group).ToList().ForEach(n => n.Rename(name));
                    break;
                case ScopeType.Group:
                    name.AddGroup();
                    Name = name.GetGroupName();
                    Children.ForEach(n => n.Rename(name));
                    break;
                case ScopeType.Core:
                    Name = name.GetCoreName();
                    name.AddCore();
                    break;
            }
        }
        #endregion

        #region Overrides
        public override string ToString()
        {
            return (String.IsNullOrEmpty(Name) ? "(Not Set)" : Name);
        }
        #endregion
    }
}
