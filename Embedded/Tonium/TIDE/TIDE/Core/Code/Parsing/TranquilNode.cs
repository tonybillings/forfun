using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;
using System.Text.RegularExpressions;
using System.Linq;

namespace TIDE.Code.Parsing
{
    public class TranquilNode : CodeTreeNode, ICodeTreeNode
    {
        #region Constructors
        public TranquilNode()
        {
            _startingLineNumber = 1;
        }
        #endregion

        #region Private Methods
        #endregion

        #region ICodeTreeNode Implementation
        public void Construct()
        {
            //_rawCode = _rawCodeStringBuilder.ToString();
            //if (String.IsNullOrEmpty(_rawCode)) return;

            //StringBuilder sb = new StringBuilder();
            //List<char> lastWord = new List<char>();
            //int currentLineNumber = _startingLineNumber;
            //int nestedLevel = 0;
            //xT50Node nestedNode = null;
            //bool clearLastWord = false;
            //int commentState = 0;

            //sb.Append(String.Concat("$", currentLineNumber, " "));

            //foreach (char c in _rawCode)
            //{
            //    switch (c)
            //    {
            //        case '{':
            //            {
            //                if (commentState > 1) continue;

            //                nestedLevel++;

            //                string word = String.Concat<char>(lastWord);
            //                ScopeType scopeType;

            //                if (!Enum.TryParse<ScopeType>(CultureInfo.CurrentCulture.TextInfo.ToTitleCase(word), out scopeType) || scopeType == ScopeType.Root)
            //                {
            //                    throw new SyntaxException("Must declare the scope type (group, core, etc) before using the scope operator.", currentLineNumber);
            //                }

            //                if (nestedNode == null)
            //                {
            //                    switch (ScopeType)
            //                    {
            //                        case ScopeType.Root:
            //                            nestedNode = new xT50Node(scopeType, currentLineNumber);
            //                            Children.Add(nestedNode);
            //                            TrimCodeEnd(sb, word);
            //                            break;
            //                        case ScopeType.Group:
            //                            if (scopeType == ScopeType.Core)
            //                            {
            //                                nestedNode = new xT50Node(scopeType, currentLineNumber);
            //                                Children.Add(nestedNode);
            //                                TrimCodeEnd(sb, word);
            //                            }
            //                            else
            //                            {
            //                                throw new SyntaxException("A 'group' code block cannot be nested within another code block.", currentLineNumber);
            //                            }
            //                            break;
            //                        case ScopeType.Core:
            //                            throw new SyntaxException("A 'core' code block cannot contain nested code blocks.", currentLineNumber);
            //                    }
            //                }
            //                else
            //                {
            //                    nestedNode.Append(c);
            //                }

            //                lastWord.Clear();
            //            }
            //            break;
            //        case '}':
            //            {
            //                if (commentState > 1) continue;

            //                nestedLevel--;

            //                if (nestedNode == null)
            //                {
            //                    throw new SyntaxException("No matching opening scope operator found.", currentLineNumber);
            //                }
            //                else
            //                {
            //                    if (nestedLevel > 0)
            //                        nestedNode.Append(c);
            //                    else
            //                        nestedNode = null;
            //                }

            //                lastWord.Clear();
            //            }
            //            break;
            //        case ' ':
            //        case '\r':
            //        case '\n':
            //        case '\t':
            //            {
            //                if (nestedNode == null)
            //                    sb.Append(c);
            //                else
            //                    nestedNode.Append(c);

            //                clearLastWord = true;

            //                if (c == '\n')
            //                {
            //                    currentLineNumber++;

            //                    if (nestedNode == null)
            //                        sb.Append("$" + currentLineNumber + " ");
            //                    else
            //                        sb.Append("\n$" + currentLineNumber + " ");

            //                    commentState = 0;
            //                }
            //            }
            //            break;
            //        case '/':
            //            {
            //                commentState++;
            //            }
            //            break;
            //        default:
            //            {
            //                if (commentState > 1) continue;

            //                if (nestedNode == null)
            //                    sb.Append(c);
            //                else
            //                    nestedNode.Append(c);

            //                if (clearLastWord)
            //                    lastWord.Clear();

            //                lastWord.Add(c);
            //                clearLastWord = false;
            //            }
            //            break;
            //    }
            //}

            //foreach (xT50Node node in Children)
            //    node.Construct();

            //_code = sb.ToString();
            //string[] codeLines = _code.Replace("\r\n", "\n").Split("\n".ToCharArray());

            //ParseConfigStatements(ref codeLines);
            //ParseConstStatements(ref codeLines);
            //ParseLabels(ref codeLines);

            //codeLines = RemoveEmptyLines(codeLines);
            //ParseInstructions(ref codeLines);

            //codeLines = RemoveEmptyLines(codeLines);
            //_code = String.Join("\r\n", codeLines);
        }

        public void Parse()
        {

        }
        #endregion
    }
}
