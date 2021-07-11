using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace TIDE.Code.Parsing
{
    public static class xT50Converter
    {
        #region Constants
        private const string DEC16_ZERO = "16'd0";
        private const string DEC8_ZERO = "8'd0";
        private const string DEC4_ZERO = "4'd0";
        private const string COMMA = ", ";
        private const string INST_END = "};";
        private const string TAB = "\t";
        private const string TAB2 = "\t\t";
        #endregion

        #region Public Methods
        public static VerilogTemplateValue[] ToVerilogTemplateValues(xT50Node node)
        {
            Dictionary<string, VerilogTemplateValue> result = new Dictionary<string, VerilogTemplateValue>();

            ParsePinConfigs(node, result);
            ParseClockConfigs(node, result);
            ParseInstructions(node, result);
            ParseCores(node, result);
            ParseProgramRoms(node, result);
            ParseExtendedMemoryModules(node, result);
            ParseSegmentDisplayDecoders(node, result);
            ParseSegmentDisplayRoms(node, result);
            ParseCombinationalLogic(node, result);

            SetDefaultClockConfig(result);

            return result.Values.ToArray();
        }
        #endregion

        #region Private Methods
        private static void ParsePinConfigs(xT50Node node, Dictionary<string,VerilogTemplateValue> values)
        {
            VerilogTemplateValue constraints = GetVerilogTemplateValue("Constraints", values);
            VerilogTemplateValue topModulePorts = GetVerilogTemplateValue("TopModulePorts", values);
            VerilogTemplateValue processorInstantiation = GetVerilogTemplateValue("ProcessorInstantiation", values);
            VerilogTemplateValue processorPorts = GetVerilogTemplateValue("ProcessorPorts", values);
            VerilogTemplateValue processorComboLogicCases = GetVerilogTemplateValue("ProcComboLogicCases", values);

            foreach (KeyValuePair<string, xT50Node.ConfigStatement> kv in node.ConfigStatements)
            {
                if (!IsPinConfigMatch(kv.Value.LeftParam)) continue;

                string pinNum = kv.Value.LeftParam.Substring(1);
                string pinMode = kv.Value.RightParam1.ToLower();
                string addr = kv.Value.RightParam2;

                string wireName = String.Concat("io_pin_", pinNum);

                constraints.PlaceholderValue += GetConstraint(pinNum, pinMode, wireName);
                topModulePorts.PlaceholderValue += GetTopModulePort(pinMode, wireName);
                processorInstantiation.PlaceholderValue += GetProcessorInstantiation(wireName);
                processorPorts.PlaceholderValue += GetProcessorPort(pinMode, wireName);

                if (pinMode == "output")
                {
                    VerilogTemplateValue processorRegisters = GetVerilogTemplateValue("ProcessorRegisters", values);
                    VerilogTemplateValue processorComboDefaults = GetVerilogTemplateValue("ProcComboLogicDefaults", values);
                    VerilogTemplateValue processorOnClockPosEdge = GetVerilogTemplateValue("SequentialLogicOnClockEdge", values);
                    VerilogTemplateValue processorOnClockPosEdgeReset = GetVerilogTemplateValue("SequentialLogicOnClockEdgeReset", values);

                    processorRegisters.PlaceholderValue += GetProcessorRegister(wireName);
                    processorComboDefaults.PlaceholderValue += GetProcessorComboLogicDefault(wireName);
                    processorOnClockPosEdge.PlaceholderValue += GetProcessorOnClockPosEdge(wireName);
                    processorOnClockPosEdgeReset.PlaceholderValue += GetProcessorOnClockPosEdgeReset(wireName);
                    processorComboLogicCases.PlaceholderValue += GetProcessorCaseForOutput(addr, wireName);
                }
                else
                {
                    processorComboLogicCases.PlaceholderValue += GetProcessorCaseForInput(addr, wireName);
                }
            }

            RemoveTrailingNewlines(values);

            foreach (xT50Node child in node.Children)
                ParsePinConfigs(child, values);
        }

        private static void ParseClockConfigs(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue customClockWire = GetVerilogTemplateValue("CustomClockWire", values);

            foreach (KeyValuePair<string, xT50Node.ConfigStatement> kv in node.ConfigStatements)
            {
                if (!IsClockConfigMatch(kv.Value.LeftParam)) continue;
                customClockWire.PlaceholderValue = String.Concat("cc_", kv.Value.RightParam1);
            }

            foreach (xT50Node child in node.Children)
                ParseClockConfigs(child, values);
        }

        private static string GetConstraint(string pinNum, string pinMode, string wireName)
        {
            string internalRestistor = ";";

            switch (pinMode)
            {
                case "input_pullup":
                    internalRestistor = " | PULLUP;";
                    break;
                case "input_pulldown":
                    internalRestistor = " | PULLDOWN;";
                    break;
            }

            string constraint = String.Concat("NET \"", wireName, "\" LOC = P", pinNum, " | IOSTANDARD = LVTTL", internalRestistor);
            return String.Concat(constraint, Environment.NewLine);
        }

        private static string GetTopModulePort(string pinMode, string wireName)
        {
            string topModulePort = String.Concat(((pinMode.Contains("input")) ? "input" : pinMode), " ", wireName, ",");
            return String.Concat(TAB, topModulePort, Environment.NewLine);
        }

        private static string GetProcessorInstantiation(string wireName)
        {
            string procInstantiation = String.Concat(".", wireName, "(", wireName, "),");
            return String.Concat(TAB, procInstantiation, Environment.NewLine);
        }

        private static string GetProcessorPort(string pinMode, string wireName)
        {
            string procPort = String.Concat(((pinMode.Contains("input")) ? "input" : pinMode), " ", wireName, ",");
            return String.Concat(TAB, procPort, Environment.NewLine);
        }

        private static string GetProcessorRegister(string wireName)
        {
            string procReg = String.Concat("reg ", wireName, "_d, ", wireName, "_q;", Environment.NewLine);
            procReg += String.Concat("assign ", wireName, " = ", wireName, "_q;");
            return String.Concat(procReg, Environment.NewLine);
        }

        private static string GetProcessorComboLogicDefault(string wireName)
        {
            string comboDefault = String.Concat(wireName, "_d = ", wireName, "_q;");
            return String.Concat(comboDefault, Environment.NewLine);
        }

        private static string GetProcessorOnClockPosEdge(string wireName)
        {
            string onClk = String.Concat(wireName, "_q <= ", wireName, "_d;");
            return String.Concat(TAB2, onClk, Environment.NewLine);
        }

        private static string GetProcessorOnClockPosEdgeReset(string wireName)
        {
            string onClkRst = String.Concat(wireName, "_q <= 0;");
            return String.Concat(TAB2, onClkRst, Environment.NewLine);
        }

        private static string GetProcessorCaseForOutput(string addr, string wireName)
        {
            string procCase = String.Concat(TAB2, addr, ":\r\n\t\tbegin\r\n\t\t\tif (core_write_$)\r\n\t\t\t\t", wireName ,"_d = |core_dout_$;\r\n\t\t\tif (core_read_$)\r\n\t\t\t\tcore_din_$ = {7'b0, ", wireName, "_q};\r\n\t\tend");
            return String.Concat(procCase, Environment.NewLine);
        }

        private static string GetProcessorCaseForInput(string addr, string wireName)
        {
            string procCase = String.Concat(TAB2, addr, ":\r\n\t\tbegin\r\n\t\t\tif (core_read_$)\r\n\t\t\t\tcore_din_$ = {7'b0, ", wireName, "};\r\n\t\tend");
            return String.Concat(procCase, Environment.NewLine);
        }

        private static void ParseInstructions(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            ReplaceConstants(node);
            ReplaceLabels(node);

            if (node.ScopeType == ScopeType.Group && node.Instructions.Count > 0)
                throw new SyntaxException("Instructions cannot be defined directly within a Group.", node.Instructions[0].LineNumber);

            if (node.Instructions.Count > 0)
            {
                VerilogTemplateValue roms = GetVerilogTemplateValue("Roms", values);

                roms.PlaceholderValue += String.Concat(VerilogTemplates.PROGRAM_ROM.Replace("$", node.Name), Environment.NewLine, Environment.NewLine);
                StringBuilder instructions = new StringBuilder();

                foreach (xT50Node.Instruction inst in node.Instructions)
                    instructions.AppendLine(ParseInstruction(inst));

                roms.PlaceholderValue = roms.PlaceholderValue.Replace("///Instructions///", instructions.ToString().TrimEnd("\r\n".ToCharArray()));
            }

            foreach (xT50Node child in node.Children)
                ParseInstructions(child, values);
        }

        private static void ReplaceConstants(xT50Node node)
        {
            foreach (xT50Node.ConstStatement constStatement in node.ConstStatements.Values)
            {
                foreach (xT50Node.Instruction inst in node.Instructions)
                {
                    if (inst.Argument0 == constStatement.LValue) inst.Argument0 = constStatement.RValue;
                    if (inst.Argument1 == constStatement.LValue) inst.Argument1 = constStatement.RValue;
                    if (inst.Argument2 == constStatement.LValue) inst.Argument2 = constStatement.RValue;
                }
            }

            foreach (xT50Node child in node.Children)
                ReplaceConstants(child);
        }

        private static void ReplaceLabels(xT50Node node)
        {
            foreach (xT50Node.LabelStatement labelStatement in node.LabelStatements.Values)
            {
                foreach (xT50Node.Instruction inst in node.Instructions)
                {
                    if (inst.Argument0 == labelStatement.Name) inst.Argument0 = labelStatement.InstructionIndex.ToString();
                    if (inst.Argument1 == labelStatement.Name) inst.Argument1 = labelStatement.InstructionIndex.ToString();
                    if (inst.Argument2 == labelStatement.Name) inst.Argument2 = labelStatement.InstructionIndex.ToString();
                }
            }

            foreach (xT50Node child in node.Children)
                ReplaceLabels(child);
        }

        private static string ParseInstruction(xT50Node.Instruction inst)
        {
            string result = null;

            SetCommandToUpper(inst);
            SetExplicitConstInst(inst);
            ReplaceGoto(inst);

            switch (inst.Command)
            {
                case "NOP":
                    result = GetInst_16Z(inst);
                    break;
                case "IN":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "INC":
                    result = GetInst_4R_4Z_8C(inst);
                    break;
                case "OUT":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "OUTC":
                    result = GetInst_4R_4Z_8C(inst);
                    break;
                case "SET":
                    result = GetInst_4R_4R_8Z(inst);
                    break;
                case "SETC":
                    result = GetInst_4R_4Z_8C(inst);
                    break;
                case "EQ":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "EQC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "NEQ":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "NEQC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "LT":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "LTC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "LTEQ":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "LTEQC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "GT":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "GTC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "GTEQ":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "GTEQC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "ADD":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "ADDC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "SUB":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "SUBC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "MUL":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "MULC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "SHL":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "SHLC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "SHR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "SHRC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "LAND":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "LANDC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "AND":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "ANDC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "NAND":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "NANDC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "LOR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "LORC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "OR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "ORC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "NOR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "NORC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "XOR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "XORC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "XNOR":
                    result = GetInst_4R_4R_4R_4Z(inst);
                    break;
                case "XNORC":
                    result = GetInst_4R_4R_8C(inst);
                    break;
                case "INV":
                    result = GetInst_4R_4R_8Z(inst);
                    break;
                case "BEQ":
                    result = GetInst_4R_4R_8Z(inst);
                    break;
                case "BEQC":
                    result = GetInst_4R_4Z_8C(inst);
                    break;
                case "BNEQ":
                    result = GetInst_4R_4R_8Z(inst);
                    break;
                case "BNEQC":
                    result = GetInst_4R_4Z_8C(inst);
                    break;
                case "DLYUS":
                    result = GetInst_16Z(inst);
                    break;
                case "DLYMS":
                    result = GetInst_16Z(inst);
                    break;
                case "SDLY":
                    result = GetInst_4C_4R_8Z(inst);
                    break;
                case "SDLYC":
                    result = GetInst_4C_4Z_8C(inst);
                    break;
                default:
                    throw new SyntaxException("Invalid or incomplete statement.", inst.LineNumber);
            }

            if (result == null) throw new SyntaxException("Invalid or incomplete statement.", inst.LineNumber);

            return result;
        }

        private static void SetCommandToUpper(xT50Node.Instruction inst)
        {
            inst.Command = inst.Command.ToUpper();
        }

        private static void SetExplicitConstInst(xT50Node.Instruction inst)
        {
            if (!inst.Command.EndsWith("C") && IsConstInstruction(inst))
                inst.Command += "C";
        }

        private static void ReplaceGoto(xT50Node.Instruction inst)
        {
            if (inst.Command == "GOTO")
            {
                inst.Command = "SETC";
                inst.Argument1 = inst.Argument0;
                inst.Argument0 = "R0";
            }
        }

        private static string GetInst_16Z(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) > 0)
                throw new SyntaxException("No arguments should be supplied for this operation.", inst.LineNumber);

            return String.Concat(GetInstStart(inst), DEC16_ZERO, INST_END);
        }

        private static string GetInst_4R_4R_8Z(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 2)
                throw new SyntaxException("Two arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetR(inst.Argument0);
            inst.Argument1 = GetR(inst.Argument1);

            if (inst.Argument0 == null || inst.Argument1 == null)
                throw new SyntaxException("All arguments must specify register by name (R1, R2, etc).", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get4D(inst.Argument1);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, inst.Argument1, COMMA, DEC8_ZERO, INST_END);
        }

        private static string GetInst_4C_4R_8Z(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 2)
                throw new SyntaxException("Two arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetC(inst.Argument0, 4);
            inst.Argument1 = GetR(inst.Argument1);

            if (inst.Argument0 == null)
                throw new SyntaxException("First argument must be a 4-bit unsigned int.", inst.LineNumber);

            if (inst.Argument1 == null)
                throw new SyntaxException("Second argument must specify register by name (R1, R2, etc).", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get4D(inst.Argument1);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, inst.Argument1, COMMA, DEC8_ZERO, INST_END);
        }

        private static string GetInst_4R_4R_4R_4Z(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 3)
                throw new SyntaxException("Three arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetR(inst.Argument0);
            inst.Argument1 = GetR(inst.Argument1);
            inst.Argument2 = GetR(inst.Argument2);

            if (inst.Argument0 == null || inst.Argument1 == null || inst.Argument2 == null)
                throw new SyntaxException("All arguments must specify register by name (R1, R2, etc).", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get4D(inst.Argument1);
            inst.Argument2 = Get4D(inst.Argument2);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, inst.Argument1, COMMA, inst.Argument2, COMMA, DEC4_ZERO, INST_END);
        }

        private static string GetInst_4R_4Z_8C(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 2)
                throw new SyntaxException("Two arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetR(inst.Argument0);
            inst.Argument1 = GetC(inst.Argument1);

            if (inst.Argument0 == null)
                throw new SyntaxException("First argument must specify register by name (R1, R2, etc).", inst.LineNumber);
            else if (inst.Argument1 == null)
                throw new SyntaxException("Second argument must be a 8-bit unsigned int or label.", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get8D(inst.Argument1);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, DEC4_ZERO, COMMA, inst.Argument1, INST_END);
        }

        private static string GetInst_4C_4Z_8C(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 2)
                throw new SyntaxException("Two arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetC(inst.Argument0, 4);
            inst.Argument1 = GetC(inst.Argument1, 8);

            if (inst.Argument0 == null)
                throw new SyntaxException("First argument must be a 4-bit unsigned int.", inst.LineNumber);

            if (inst.Argument1 == null)
                throw new SyntaxException("Second argument must be a 8-bit unsigned int.", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get8D(inst.Argument1);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, DEC4_ZERO, COMMA, inst.Argument1, INST_END);
        }

        private static string GetInst_4R_4R_8C(xT50Node.Instruction inst)
        {
            if (GetInstOperandCount(inst) != 3)
                throw new SyntaxException("Three arguments are required for this operation.", inst.LineNumber);

            inst.Argument0 = GetR(inst.Argument0);
            inst.Argument1 = GetR(inst.Argument1);
            inst.Argument2 = GetC(inst.Argument2);

            if (inst.Argument0 == null || inst.Argument1 == null)
                throw new SyntaxException("First and second argument must specify register by name (R1, R2, etc).", inst.LineNumber);
            else if (inst.Argument2 == null)
                throw new SyntaxException("Third argument must be a 8-bit unsigned int or label.", inst.LineNumber);

            inst.Argument0 = Get4D(inst.Argument0);
            inst.Argument1 = Get4D(inst.Argument1);
            inst.Argument2 = Get8D(inst.Argument2);

            return String.Concat(GetInstStart(inst), inst.Argument0, COMMA, inst.Argument1, COMMA, inst.Argument2, INST_END);
        }

        private static string GetR(string argument)
        {
            string result = null;

            if (RegisterMatch(argument))
                result = argument.Substring(1);

            return result;
        }

        private static string GetC(string argument, int bitLength = 8)
        {
            string result = null;

            int i = -1;

            if (DecimalMatch(argument))
                Int32.TryParse(argument, out i);
            else if (HexadecimalMatch(argument))
                Int32.TryParse(argument.Substring(2), NumberStyles.AllowHexSpecifier, CultureInfo.InvariantCulture, out i);
            else if (BinaryMatch(argument))
                i = Convert.ToInt32(argument.ToString().ToLower().Replace("b", ""), 2);

            if (i > -1 && i < Math.Pow(2, bitLength))
                result = i.ToString();

            return result;
        }

        private static string Get4D(string argument)
        {
            return String.Concat("4'd", argument);
        }

        private static string Get8D(string argument)
        {
            return String.Concat("8'd", argument);
        }

        private static int GetInstOperandCount(xT50Node.Instruction inst)
        {
            int count = 0;

            if (!String.IsNullOrEmpty(inst.Argument0)) count++;
            if (!String.IsNullOrEmpty(inst.Argument1)) count++;
            if (!String.IsNullOrEmpty(inst.Argument2)) count++;

            return count;
        }

        private static string GetInstStart(xT50Node.Instruction inst)
        {
            return String.Concat(TAB2, "8'd", inst.Index, ": inst = {`", inst.Command, ", ");
        }

        private static bool IsConstInstruction(xT50Node.Instruction inst)
        {
            if (inst.Command == "IN" || inst.Command == "OUT")
            {
                if (!String.IsNullOrEmpty(inst.Argument1) && !RegisterMatch(inst.Argument1)) return true;
            }
            else
            {
                if (!String.IsNullOrEmpty(inst.Argument1) && !RegisterMatch(inst.Argument1)) return true;
                if (!String.IsNullOrEmpty(inst.Argument2) && !RegisterMatch(inst.Argument2)) return true;
            }

            return false;
        }

        private static bool IsPinConfigMatch(string text)
        {
            Regex regex = new Regex(@"(p)[0-9]*");
            Match match = regex.Match(text.ToLower());
            return match.Success && match.Length == text.Length;
        }

        private static bool IsClockConfigMatch(string text)
        {
            Regex regex = new Regex(@"(clock)");
            Match match = regex.Match(text.ToLower());
            return match.Success && match.Length == text.Length;
        }

        private static bool RegisterMatch(string text)
        {
            Regex regex = new Regex(@"(R|r)[0-9]+");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private static bool ConstantMatch(string text)
        {
            Regex regex = new Regex(@"[A-Za-z0-9_]+");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private static bool DecimalMatch(string text)
        {
            Regex regex = new Regex(@"[\d]+");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private static bool HexadecimalMatch(string text)
        {
            Regex regex = new Regex(@"0[xX][0-9a-fA-F]+");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private static bool BinaryMatch(string text)
        {
            Regex regex = new Regex(@"[01]{1,8}[bB]");
            Match match = regex.Match(text);
            return match.Success && match.Length == text.Length;
        }

        private static void ParseCores(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue cores = GetVerilogTemplateValue("Cores", values);
            VerilogTemplateValue processorOnClockPosEdge = GetVerilogTemplateValue("SequentialLogicOnClockEdge", values);
            VerilogTemplateValue processorOnClockPosEdgeReset = GetVerilogTemplateValue("SequentialLogicOnClockEdgeReset", values);

            if (node.Instructions.Count > 0)
            {
                string core = VerilogTemplates.CORE_INSTANTIATION.Replace("$", node.Name);
                cores.PlaceholderValue += String.Concat(core, Environment.NewLine, Environment.NewLine);
            }

            foreach (xT50Node child in node.Children)
                ParseCores(child, values);
        }

        private static void ParseProgramRoms(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue progRoms = GetVerilogTemplateValue("ProgramRoms", values);

            if (node.Instructions.Count > 0)
            {
                string progRom = VerilogTemplates.PROGRAM_ROM_INSTANTIATION.Replace("$", node.Name);
                progRoms.PlaceholderValue += String.Concat(progRom, Environment.NewLine, Environment.NewLine);
            }

            foreach (xT50Node child in node.Children)
                ParseProgramRoms(child, values);
        }

        private static void ParseExtendedMemoryModules(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue extMemMods = GetVerilogTemplateValue("ExtendedMemoryModules", values);

            if (node.ScopeType == ScopeType.Root)
            {
                if (node.Instructions.Count > 0)
                {
                    string extMemMod = VerilogTemplates.EXTENDED_MEMORY_INSTANTIATION
                        .Replace("$ST $N", "Root Core")
                        .Replace("$N", "0")
                        .Replace("$C1", node.Name)
                        .Replace("$C2", String.Concat(node.Name, "_NA"))
                        .Replace("$0", " = 1'bz")
                        .Replace("$1", " = 8'bzzzzzzzz");

                    extMemMods.PlaceholderValue += String.Concat(extMemMod, Environment.NewLine, Environment.NewLine);
                }

                foreach (xT50Node child in node.Children.Where(c => c.ScopeType == ScopeType.Core))
                {
                    if (child.Instructions.Count == 0) continue;

                    string extMemMod = VerilogTemplates.EXTENDED_MEMORY_INSTANTIATION
                        .Replace("$ST", "Core")
                        .Replace("$N", child.Name)
                        .Replace("$C1", child.Name)
                        .Replace("$C2", String.Concat(child.Name, "_NA"))
                        .Replace("$0", " = 1'bz")
                        .Replace("$1", " = 8'bzzzzzzzz");

                    extMemMods.PlaceholderValue += String.Concat(extMemMod, Environment.NewLine, Environment.NewLine);
                }
            }
            else if (node.ScopeType == ScopeType.Group)
            {

                xT50Node child1 = (node.Children.Count > 0) ? (xT50Node)node.Children[0] : null;
                xT50Node child2 = (node.Children.Count > 1) ? (xT50Node)node.Children[1] : null;

                string extMemMod = VerilogTemplates.EXTENDED_MEMORY_INSTANTIATION.Replace("$ST", "Group").Replace("$N", node.Name);

                if (child1 != null)
                {
                    extMemMod = extMemMod.Replace("$C1", child1.Name);

                    if (child2 != null)
                        extMemMod = extMemMod.Replace("$C2", child2.Name).Replace("$0", "").Replace("$1", "");
                    else
                        extMemMod = extMemMod.Replace("$C2", String.Concat(child1.Name, "_NA")).Replace("$0", " = 1'bz").Replace("$1", " = 8'bzzzzzzzz");

                    extMemMods.PlaceholderValue += String.Concat(extMemMod, Environment.NewLine, Environment.NewLine);
                }
            }

            foreach (xT50Node child in node.Children)
                ParseExtendedMemoryModules(child, values);
        }
        
        private static void ParseSegmentDisplayDecoders(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue segDispDecs = GetVerilogTemplateValue("SegmentDisplayDecoders", values);

            if (node.Instructions.Count > 0)
            {
                string segDispDec = VerilogTemplates.SEGMENT_DISPLAY_DECODER_INSTANTIATION.Replace("$", node.Name);
                segDispDecs.PlaceholderValue += String.Concat(segDispDec, Environment.NewLine, Environment.NewLine);
            }

            foreach (xT50Node child in node.Children)
                ParseSegmentDisplayDecoders(child, values);
        }

        private static void ParseSegmentDisplayRoms(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue segDispRoms = GetVerilogTemplateValue("SegmentDisplayRoms", values);

            if (node.Instructions.Count > 0)
            {
                string segDispRom = VerilogTemplates.SEGMENT_DISPLAY_ROM_INSTANTIATION.Replace("$", node.Name);
                segDispRoms.PlaceholderValue += String.Concat(segDispRom, Environment.NewLine, Environment.NewLine);
            }

            foreach (xT50Node child in node.Children)
                ParseSegmentDisplayRoms(child, values);
        }

        private static void ParseCombinationalLogic(xT50Node node, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue combinationalLogic = GetVerilogTemplateValue("CombinationalLogic", values);
            VerilogTemplateValue procComboLogicCases = GetVerilogTemplateValue("ProcComboLogicCases", values);

            if (node.Instructions.Count > 0)
            {
                string comboLogic = VerilogTemplates.PROCESSOR_COMBINATIONAL_LOGIC
                    .Replace(String.Concat("///", procComboLogicCases.PlaceholderText, "///"), procComboLogicCases.PlaceholderValue)
                    .Replace("$", node.Name);

                combinationalLogic.PlaceholderValue += String.Concat(comboLogic, Environment.NewLine, Environment.NewLine);
            }

            foreach (xT50Node child in node.Children)
                ParseCombinationalLogic(child, values);
        }

        private static void RemoveTrailingNewlines(Dictionary<string, VerilogTemplateValue> values)
        {
            foreach (KeyValuePair<string, VerilogTemplateValue> kv in values)
                kv.Value.PlaceholderValue = (kv.Value.PlaceholderValue == null) ? "" : kv.Value.PlaceholderValue.TrimEnd(Environment.NewLine.ToCharArray());
        }

        private static VerilogTemplateValue GetVerilogTemplateValue(string name, Dictionary<string, VerilogTemplateValue> values)
        {
            VerilogTemplateValue value = null;

            if (!values.TryGetValue(name, out value))
            {
                value = new VerilogTemplateValue() { PlaceholderText = name };
                values.Add(name, value);
            }

            return value;
        }

        private static void SetDefaultClockConfig(Dictionary<string, VerilogTemplateValue> values)
        {
            foreach (KeyValuePair<string, VerilogTemplateValue> kv in values)
            {
                if (kv.Key == "CustomClockWire" && kv.Value.PlaceholderValue == null)
                    kv.Value.PlaceholderValue = String.Concat("cc_", VerilogTemplates.DEFAULT_CLOCK_SPEED);
            }
        }
        #endregion
    }
}
