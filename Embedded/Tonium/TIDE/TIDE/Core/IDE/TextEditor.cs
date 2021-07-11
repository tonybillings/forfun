using ICSharpCode.AvalonEdit.Highlighting;
using System.Collections.Generic;
using System.Windows.Media;

namespace TIDE.IDE
{
    public class TextEditor : ICSharpCode.AvalonEdit.TextEditor
    {
        #region Public Variables
        public Dictionary<string, IHighlightingDefinition> HighlightingDefinitions { get; set; }
        #endregion

        #region Constructors
        public TextEditor() : base()
        {
            HighlightingDefinitions = new Dictionary<string, IHighlightingDefinition>();
        }
        #endregion

        #region Public Methods
        public void SetOptions()
        {
            IDE.Options.Set("TextEditor.FontFamily", FontFamily.ToString());
            IDE.Options.Set("TextEditor.FontSize", FontSize);
        }

        public void GetOptions()
        {
            string fontFamily = IDE.Options.Get<string>("TextEditor.FontFamily");
            FontFamily = new FontFamily(fontFamily);

            FontSize = IDE.Options.Get<double>("TextEditor.FontSize");
        }
        #endregion
    }
}
