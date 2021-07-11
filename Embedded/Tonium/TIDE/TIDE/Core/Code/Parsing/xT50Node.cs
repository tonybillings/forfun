using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.Text.RegularExpressions;
using System.Linq;

namespace TIDE.Code.Parsing
{
    public class xT50Node : CodeTreeNode, ICodeTreeNode
    {
        #region Nested Types
        public class ConfigStatement
        {
            public string LeftParam { get; set; }
            public string RightParam1 { get; set; }
            public string RightParam2 { get; set; }
        }

        public class ConstStatement
        {
            public string LValue { get; set; }
            public string RValue { get; set; }
        }

        public class LabelStatement
        {
            public string Name { get; set; }
            public int InstructionIndex { get; set; }
        }

        public class Instruction
        {
            public int Index { get; set; }
            public int LineNumber { get; set; }
            public string Command { get; set; }
            public string Argument0 { get; set; }
            public string Argument1 { get; set; }
            public string Argument2 { get; set; }
        }
        #endregion

        #region Private Variables
        private Dictionary<string, ConfigStatement> _configStatements;
        private Dictionary<string, ConstStatement> _constStatements;
        private Dictionary<string, LabelStatement> _labelStatements;
        private List<Instruction> _instructions;
        #endregion

        #region Properties
        public Dictionary<string, ConfigStatement> ConfigStatements { get { return _configStatements; } }

        public Dictionary<string, ConstStatement> ConstStatements { get { return _constStatements; } }

        public Dictionary<string, LabelStatement> LabelStatements { get { return _labelStatements; } }

        public List<Instruction> Instructions { get { return _instructions; } }
        #endregion

        #region Constructors
        public xT50Node()
        {
            _configStatements = new Dictionary<string, ConfigStatement>();
            _constStatements = new Dictionary<string, ConstStatement>();
            _labelStatements = new Dictionary<string, LabelStatement>();
            _instructions = new List<Instruction>();
            _startingLineNumber = 1;
        }
        #endregion

        #region Private Methods
        private xT50Node GetNode(string name) 
        {
            if (String.IsNullOrEmpty(name)) return null;
            if (Name == name) return this;

            foreach (xT50Node child in Children)
            {
                xT50Node result = child.GetNode(name);
                if (result != null) return result;
            }

            return null;
        }

        private void ParseConfigStatements(ref string[] codeLines)
        {
            _configStatements = new Dictionary<string, ConfigStatement>();

            for (int i = 0; i < codeLines.Length; i++)
            {
                string line = codeLines[i];
                string lineNumber = line.Substring(1, line.IndexOf(" ") - 1);
                string subLine = line.Substring(line.IndexOf(' ')).Trim();

                if (subLine.StartsWith("[") && subLine.EndsWith("]"))
                {
                    ConfigStatement cs = null;

                    try
                    {
                        cs = GetConfigStatement(subLine);
                    }
                    catch (SyntaxException ex)
                    {
                        ex.LineNumber = Convert.ToInt32(lineNumber);
                        throw ex;
                    }
                    catch (Exception)
                    {
                        throw new SyntaxException("Invalid or incomplete configuration statement.", Convert.ToInt32(lineNumber));
                    }

                    try
                    {
                        _configStatements.Add(cs.LeftParam, cs);
                        codeLines[i] = String.Concat("$", lineNumber, " ");
                    }
                    catch
                    {
                        throw new SyntaxException("Configuration for '" + cs.LeftParam + "' already defined.", Convert.ToInt32(lineNumber));
                    }
                } 
            }

            _configStatements.Reverse();
        }

        private void ParseConstStatements(ref string[] codeLines)
        {
            _constStatements = new Dictionary<string, ConstStatement>();
            int currentLineNumber = _startingLineNumber;

            for (int i = 0; i < codeLines.Length; i++)
            {
                string line = codeLines[i];
                string subLine = line.Substring(line.IndexOf(' ')).Trim();
                string lineNumber = line.Substring(1, line.IndexOf(" ") - 1);

                if (IsConstStatementMatch(subLine.ToLower()))
                {
                    ConstStatement cs = null;

                    try
                    {
                        cs = GetConstStatement(subLine);
                    }
                    catch
                    {
                        throw new SyntaxException("Invalid or incomplete const statement.", Convert.ToInt32(lineNumber));
                    }

                    try
                    {
                        AddConstStatement(cs);
                        codeLines[i] = String.Concat("$", lineNumber, " ");
                    }
                    catch
                    {
                        throw new SyntaxException("Const '" + cs.LValue + "' already defined.", Convert.ToInt32(lineNumber));
                    }
                }
            }
        }

        private void ParseLabels(ref string[] codeLines)
        {
            _labelStatements = new Dictionary<string, LabelStatement>();

            int currentLineNumber = _startingLineNumber;
            int currentInstruction = 0;

            for (int i = 0; i < codeLines.Length; i++)
            {
                string line = codeLines[i];
                string lineNumber = line.Substring(1, line.IndexOf(" ") - 1);
                string subLine = line.Substring(line.IndexOf(' ')).Trim();

                if (String.IsNullOrEmpty(subLine)) continue;

                if (IsLabelMatch(subLine))
                {
                    subLine = subLine.Replace(" ", "").Replace(":", "");

                    LabelStatement ls = new LabelStatement() { Name = subLine, InstructionIndex = currentInstruction };

                    try
                    {
                        _labelStatements.Add(ls.Name, ls);
                        codeLines[i] = String.Concat("$", lineNumber, " ");
                    }
                    catch
                    {
                        throw new SyntaxException("Label '" + ls.Name + "' already defined.", Convert.ToInt32(lineNumber));
                    }
                }
                else
                {
                    currentInstruction++;
                }
            }

            RemoveEmptyLines(ref codeLines);
        }

        private void ParseInstructions(ref string[] codeLines)
        {
            for (int i = 0; i < codeLines.Length; i++)
            {
                string line = codeLines[i];
                if (String.IsNullOrEmpty(line)) continue;
                int lineNumber = Convert.ToInt32(line.Substring(1, line.IndexOf(" ") - 1));
                string instLine = line.Substring(line.IndexOf(' ')).Trim();
                if (String.IsNullOrEmpty(instLine)) continue;
                string[] instWords = instLine.Replace(",", "").Replace("\t", "").Split(" ".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                codeLines[i] = line.Substring(0, line.IndexOf(' ') + 1);
                Instruction inst = new Instruction() { LineNumber = lineNumber, Index = i };

                switch (instWords.Length)
                {
                    case 4:
                        inst.Argument2 = instWords[3];
                        inst.Argument1 = instWords[2];
                        inst.Argument0 = instWords[1];
                        inst.Command = instWords[0];
                        break;
                    case 3:
                        inst.Argument1 = instWords[2];
                        inst.Argument0 = instWords[1];
                        inst.Command = instWords[0];
                        break;
                    case 2:
                        inst.Argument0 = instWords[1];
                        inst.Command = instWords[0];
                        break;
                    case 1:
                        inst.Command = instWords[0];
                        break;
                }

                _instructions.Add(inst);
            }

            RemoveEmptyLines(ref codeLines);

            foreach (xT50Node child in Children)
                child.ParseInstructions(ref codeLines);
        }

        private bool IsGroupScopeConfigMatch(string text)
        {
            Regex regex = new Regex(@"[A-Za-z0-9_]+\.\*::");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsCoreScopeConfigMatch(string text)
        {
            Regex regex = new Regex(@"[A-Za-z0-9_]+::");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsGroupCoreScopeConfigMatch(string text)
        {
            Regex regex = new Regex(@"[A-Za-z0-9_]+\.[A-Za-z0-9_]+::");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsPinConfigMatch(string text)
        {
            Regex regex = new Regex(@"\[[ ]*(p)[0-9]*[ ]*\=[ ]*(input|input_pullup|input_pulldown|output)[ ]*\,[ ]*[0-9]+[ ]*\]");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsClockConfigMatch(string text)
        {
            Regex regex = new Regex(@"\[[ ]*(clock)[ ]*\=[ ]*[0-9]+[ ]*\]");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsConstStatementMatch(string text)
        {
            Regex regex = new Regex(@"(const)[ ]+[A-Za-z0-9_]+[ ]*\=[ ]*[A-Za-z0-9_]+");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private bool IsLabelMatch(string text)
        {
            Regex regex = new Regex(@"[A-Za-z0-9_]+\:");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private ConfigStatement GetConfigStatement(string line)
        {
            line = line.ToLower();

            if (IsPinConfigMatch(line))
            {
                return GetPinConfigStatement(line);
            }
            else if (IsClockConfigMatch(line))
            {
                return GetClockConfigStatement(line);
            }

            throw new Exception();
        }

        private ConfigStatement GetPinConfigStatement(string line)
        {
            ConfigStatement cs = new ConfigStatement();

            line = line.Substring(1, line.Length - 2).Replace(" ", "");
            string[] paramArray = line.Split("=".ToCharArray());
            string[] rightParams = paramArray[1].Split(",".ToCharArray());

            cs.LeftParam = paramArray[0];
            cs.RightParam1 = rightParams[0];
            cs.RightParam2 = rightParams[1];

            return cs;
        }

        private ConfigStatement GetClockConfigStatement(string line)
        {
            ConfigStatement cs = new ConfigStatement();

            line = line.Substring(1, line.Length - 2).Replace(" ", "");
            string[] paramArray = line.Split("=".ToCharArray());

            cs.LeftParam = paramArray[0];
            cs.RightParam1 = paramArray[1];

            int speed;

            if (!Int32.TryParse(cs.RightParam1, out speed) || !VerilogTemplates.CLOCK_SPEEDS.Contains(speed))
            {
                string[] speeds = VerilogTemplates.CLOCK_SPEEDS.Select(i => i.ToString()).ToArray();
                speeds[speeds.Length - 1] = String.Concat("and ", speeds[speeds.Length - 1]);
                throw new SyntaxException(String.Concat("Invalid clock speed specified.\r\nValid values (in MHz) are ", String.Join(", ", speeds), "."), -1);
            }

            return cs;
        }

        private ConstStatement GetConstStatement(string line)
        {           
            ConstStatement cs = new ConstStatement();

            string[] lineArray = line.Split(" ".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);

            cs.LValue = lineArray[1];
            cs.RValue = lineArray[3];

            return cs;
        }

        private void RemoveEmptyLines(ref string[] codeLines)
        {
            List<string> result = new List<string>();

            foreach (string line in codeLines)
            {
                string l = line.Substring(line.IndexOf(' ')).Trim();

                if (!String.IsNullOrEmpty(l))
                    result.Add(line);
            }

            codeLines = result.ToArray();
        }

        private void SetLineNumbers(ref string[] codeLines)
        {
            StringBuilder sb = new StringBuilder();
            int currentLineNumber = _startingLineNumber;

            foreach (string line in codeLines)
            {
                if (!String.IsNullOrEmpty(line.Trim()))
                    sb.Append(String.Concat("$", currentLineNumber, " ", line.Trim(), Environment.NewLine));

                currentLineNumber++;
            }

            _code = sb.ToString();
            codeLines = _code.Replace("\r\n", "\n").Split("\n".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
        }

        private void TrimCodeEnd(StringBuilder sb, string trimValue)
        {
            int index = sb.ToString().LastIndexOf(trimValue);
            int length = sb.Length - index;
            sb.Remove(index, length);
        }

        private void AddConstStatement(ConstStatement constStatement)
        {
            _constStatements.Add(constStatement.LValue, constStatement);

            foreach (xT50Node node in Children)
                node.AddConstStatement(constStatement);
        }

        private string GetCoreNameFromScopeStatement(string scopeStatement)
        {
            string[] scopeNames = scopeStatement.Substring(0, scopeStatement.IndexOf(':')).Split(".".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
            return (scopeNames[scopeNames.Length - 1] == "*") ? null : scopeNames[scopeNames.Length - 1];
        }

        private string GetGroupNameFromScopeStatement(string scopeStatement)
        {
            return scopeStatement.Substring(0, scopeStatement.IndexOf(':')).Split(".".ToCharArray(), StringSplitOptions.RemoveEmptyEntries)[0];
        }
        #endregion

        #region ICodeTreeNode Implementation
        public void Construct()
        {
            _rawCode = _rawCodeStringBuilder.ToString();

            StringBuilder sb = new StringBuilder();
            string[] codeLines = _rawCode.Replace("\r\n", "\n").Split("\n".ToCharArray());
            xT50Node currentNode = this;
            int currentLineNumber = _startingLineNumber;

            foreach (string line in codeLines)
            {
                string scopeStatement = line.Trim();

                if (scopeStatement.EndsWith("::"))
                {
                    if (IsCoreScopeConfigMatch(scopeStatement))
                    {
                        string coreName = GetCoreNameFromScopeStatement(scopeStatement);
                        xT50Node coreNode = GetNode(coreName);

                        if (coreNode != null && coreNode.ScopeType == ScopeType.Core)
                            throw new SyntaxException("A core with this name has already been defined.", currentLineNumber);

                        coreNode = new xT50Node() { ScopeType = ScopeType.Core, Name = coreName, _startingLineNumber = currentLineNumber + 1 };
                        Children.Add(coreNode);
                        currentNode = coreNode;
                    }
                    else if (IsGroupScopeConfigMatch(scopeStatement))
                    {
                        string groupName = GetGroupNameFromScopeStatement(scopeStatement);
                        xT50Node groupNode = GetNode(groupName);

                        if (groupNode != null && groupNode.ScopeType == ScopeType.Group && groupNode._startingLineNumber != -1)
                            throw new SyntaxException("A group with this name has already been defined.", currentLineNumber);

                        if (groupNode == null)
                        {
                            groupNode = new xT50Node() { ScopeType = ScopeType.Group, Name = groupName, _startingLineNumber = currentLineNumber + 1 };
                            Children.Add(groupNode);
                        }
                        else
                        {
                            groupNode._startingLineNumber = currentLineNumber + 1;
                        }
                        
                        currentNode = groupNode;
                    }
                    else if (IsGroupCoreScopeConfigMatch(scopeStatement))
                    {
                        string coreName = GetCoreNameFromScopeStatement(scopeStatement);
                        string groupName = GetGroupNameFromScopeStatement(scopeStatement);
                        xT50Node coreNode = GetNode(String.Concat(groupName, ".", coreName));

                        if (coreNode != null && coreNode.ScopeType == ScopeType.Core)
                            throw new SyntaxException("A core with this name in this group has already been defined.", currentLineNumber);

                        xT50Node groupNode = GetNode(groupName);

                        if (groupNode == null)
                        {
                            groupNode = new xT50Node() { ScopeType = ScopeType.Group, Name = groupName, _startingLineNumber = -1 };
                            Children.Add(groupNode);
                        }

                        coreNode = new xT50Node() { ScopeType = ScopeType.Core, Name = String.Concat(groupName, ".", coreName), _startingLineNumber = currentLineNumber + 1 };
                        groupNode.Children.Add(coreNode);
                        currentNode = coreNode;
                    }
                }
                else
                {
                    string uncommentedCode = (line.Contains("#")) ? line.Substring(0, line.IndexOf("#")) : line;
                    uncommentedCode = uncommentedCode.Trim();

                    if (currentNode == this)
                        sb.Append(String.Concat(uncommentedCode, Environment.NewLine));
                    else
                        currentNode.Append(String.Concat(uncommentedCode, Environment.NewLine));
                }

                currentLineNumber++;
            }

            _code = sb.ToString();

            foreach (xT50Node child in Children)
                child.Construct();
        }

        public void Parse()
        {
            foreach (xT50Node child in Children)
                child.Parse();

            if (String.IsNullOrEmpty(_code)) return;
            string[] codeLines = _code.Replace("\r\n", "\n").Split("\n".ToCharArray());

            SetLineNumbers(ref codeLines);
            ParseConfigStatements(ref codeLines);
            ParseConstStatements(ref codeLines);
            ParseLabels(ref codeLines);      
            ParseInstructions(ref codeLines);
      
            _code = String.Join("\r\n", codeLines);
        }
        #endregion
    }
}
