using System;
using System.Collections.Generic;
using System.Text;

namespace ExpressionAnalisys {
    public interface IFunction {
        double GetValue(double firstValue, params double[] inputVector);
    }

    [Serializable]
    public class Expression : IFunction {
        private const int MinStackSize = 10;
        private const char StartOperandsCode = '\u1200';
        private const string Oper = "+-/*\u1100\u1101\u1102\u1103\u1104\u1105\u1106\u1107\u1108\u1109\u1110\u1111\u1112^(";
        private const string NonCodeOper = "+-/*^)(";
        private static readonly int[] Prior = {1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0};
        private readonly Stack<double> _stackResult;
		private readonly Stack<char> _stackOperation;
        private readonly Dictionary<string, double> _constsDictionary;
		private string _reversStr;
        private string _operands;
        private static readonly Dictionary<string, char> FunctionDictionary;

        static Expression() {
		    FunctionDictionary = new Dictionary<string, char>(15);
            FunctionDictionary.Add("sin", '\u1100');
            FunctionDictionary.Add("cos", '\u1101');
            FunctionDictionary.Add("tg", '\u1102');
            FunctionDictionary.Add("ln", '\u1103');
            FunctionDictionary.Add("exp", '\u1104');
            FunctionDictionary.Add("abs", '\u1105');
            FunctionDictionary.Add("arcsin", '\u1106');
            FunctionDictionary.Add("arccos", '\u1107');
            FunctionDictionary.Add("arcctg", '\u1108');
            FunctionDictionary.Add("ch", '\u1109');
            FunctionDictionary.Add("sh", '\u1110');
            FunctionDictionary.Add("th", '\u1111');
            FunctionDictionary.Add("sign", '\u1112');
            FunctionDictionary.Add("e", '\u1113');
            FunctionDictionary.Add("pi", '\u1114');
		}
        
        public Expression() {
			_stackResult = new Stack<double>(MinStackSize);
			_stackOperation = new Stack<char>(MinStackSize);
            _constsDictionary = new Dictionary<string, double>();
		}

        public Expression(string str, string[] operands) : this() {
            SetExpression(str, operands);
        }

		public void SetExpression (string str, string[] operands) {
		    var operandsDictionary = BuildOperandsDictionary(operands);
			var preparedStr = Preparation(str, operandsDictionary);
			_reversStr = BuildPol(preparedStr);
		}

        public void AddConstant (string name, double value) {
            if (_constsDictionary.ContainsKey(name)) {
                _constsDictionary[name] = value;
            }
            else {
                _constsDictionary.Add(name, value);
            }
        }

        public double GetValue (double firstValue, params double[] inputVector) {
		    var i = 0;
            do {
			    var operandIndex = _operands.IndexOf(_reversStr[i]);
                if (operandIndex == 0) {
                    _stackResult.Push(firstValue);
				}
                else if (operandIndex > 0) {
                    _stackResult.Push(inputVector[operandIndex - 1]);
                }
                else if (_reversStr[i] == '\u1113') {
                    _stackResult.Push(Math.E);
                }
                else if (_reversStr[i] == '\u1114') {
                    _stackResult.Push(Math.PI);
                }
				else {
					var posInOper = Oper.IndexOf(_reversStr[i]);
					if (posInOper >= 0) {
					    double operation1;
					    if ((Prior[posInOper] == 3) && (_reversStr[i] != '^')) {
							operation1 = _stackResult.Pop();
                            _stackResult.Push(Calc(_reversStr[i], operation1));
						}
						else {
							operation1 = _stackResult.Pop();
							double operation2 = _stackResult.Pop();
                            _stackResult.Push(Calc(_reversStr[i], operation2, operation1));
						}
					}
					else if (_reversStr[i] != ' ') {
						var j = i;
						do {
							i++;
						} while ( (i < _reversStr.Length) && (_reversStr[i] != ' ') );
                        _stackResult.Push(Convert.ToDouble(_reversStr.Substring(j, i - j)));
					}
				}
			    i++;
			} while (i < _reversStr.Length);
			return _stackResult.Pop();
		}

        private Dictionary<string, char> BuildOperandsDictionary(string[] operands) {
            var curCode = StartOperandsCode;
            var size = operands.Length;
            var stringBuilder = new StringBuilder(size);
            var operandsDictionary = new Dictionary<string, char>(size);
            for (int i = 0; i < size; i++) {
                var operand = operands[i];
                stringBuilder.Append(curCode);
                operandsDictionary.Add(operand, curCode);
                curCode++;
            }
            _operands = stringBuilder.ToString();
            return operandsDictionary;
        }

		private string Preparation(string str, Dictionary<string, char> operands) {
			var tmpMas = new char[10];
            var stringBuilder = new StringBuilder(str);
			int i = 0;
			if (stringBuilder[0] == '-') {
			    stringBuilder.Insert(0, '0');
			}
		    while (i < stringBuilder.Length) {
				var curSymbol = stringBuilder[i];
				if ((curSymbol == '(') && (stringBuilder[i + 1] == '-')) {
				    stringBuilder.Insert(i + 1, '0');
				}
				else {
				    if (NonCodeOper.IndexOf(curSymbol) < 0) {
				        var j = i + 1;
                        while ((j < stringBuilder.Length) && (NonCodeOper.IndexOf(stringBuilder[j]) < 0)) {
                            j++;
                        }
				        stringBuilder.CopyTo(i, tmpMas, 0, j - i);
                        stringBuilder.Remove(i, j - i);
				        var operName = new string(tmpMas, 0, j - i);

				        char code;
                        double constValue;
				        if (FunctionDictionary.TryGetValue(operName, out code)) {
				            stringBuilder.Insert(i, code);
				        }
				        else if (operands.TryGetValue(operName, out code)) {
				            stringBuilder.Insert(i, code);
				        }
				        else if (_constsDictionary.TryGetValue(operName, out constValue)) {
				            if (constValue >= 0.0) {
				                var value = constValue.ToString();
                                stringBuilder.Insert(i, value);
				                i += value.Length - 1;
				            }
				            else {
				                var value = "(0" + constValue.ToString() + ")";
                                stringBuilder.Insert(i, value);
				                i += value.Length - 1;
				            }
				        }
                        else {
				            try {
				                Convert.ToDouble(operName);
                                stringBuilder.Insert(i, operName);
				            }
				            catch (Exception) {
				                throw new ArgumentException("Unknow operation or operand: " + operName);
				            }
				        }
				    }
				}
			    i++;
			}
			return stringBuilder.ToString();
		}
		
		private string BuildPol(string str) {
			var stringBuilder = new StringBuilder(50);
			var ff = false;
			foreach (char tmp in str) {
				if ( (Oper.IndexOf(tmp) < 0) && (tmp != ')') ) {
					if (ff)
						stringBuilder.Append(' ');
					stringBuilder.Append(tmp);
					ff = false;
				}
				else {
					ff = true;
					if (tmp == '(')
						_stackOperation.Push('(');
					else
						if (tmp == ')') {
							while (_stackOperation.Peek() != '(' ) {
								stringBuilder.Append(' ');
								stringBuilder.Append(_stackOperation.Pop());
							}
							_stackOperation.Pop();
						}
						else {
							while ( (_stackOperation.Count > 0) && ( Prior[Oper.IndexOf(_stackOperation.Peek())] >= Prior[Oper.IndexOf(tmp)] ) ) {
								stringBuilder.Append(' ');
								stringBuilder.Append(_stackOperation.Pop());
							}
							_stackOperation.Push(tmp);
						}
				}
			}
			while (_stackOperation.Count != 0) {
				stringBuilder.Append(' ');
				stringBuilder.Append(_stackOperation.Pop());
			}

			return stringBuilder.ToString();
		}

		private static double Calc(char s, double x, double y = 0) {
			double rez = 0;
			switch (s) {
			    case '+': rez = x + y; break;
			    case '-': rez = x - y; break;
			    case '/': rez = x / y; break;
			    case '*': rez = x * y; break;
			    case '^': rez = Math.Pow(x,y); break;
			    case '\u1100': rez = Math.Sin(x); break;
			    case '\u1101': rez = Math.Cos(x); break;
			    case '\u1102': rez = Math.Tan(x); break;
			    case '\u1103': rez = Math.Log(x); break;
			    case '\u1104': rez = Math.Exp(x); break;
			    case '\u1105': rez = Math.Abs(x); break;
                case '\u1106': rez = Math.Asin(x); break;
			    case '\u1107': rez = Math.Acos(x); break;
			    case '\u1108': rez = Math.Atan(x); break;
			    case '\u1109': rez = Math.Cosh(x); break;
			    case '\u1110': rez = Math.Sinh(x); break;
			    case '\u1111': rez = Math.Tanh(x); break;
			    case '\u1112': rez = Math.Sign(x); break;
			}
			return rez;
		}
	}
}