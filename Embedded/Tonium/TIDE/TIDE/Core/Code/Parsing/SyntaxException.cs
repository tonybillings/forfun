using System;

namespace TIDE.Code.Parsing
{
    public class SyntaxException : Exception
    {
        public int LineNumber { get; set; }

        public SyntaxException(string message, int lineNumber) : base(message)
        {
            LineNumber = lineNumber;
        }
    }
}
