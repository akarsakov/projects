using System;
using Alchemi.Core;
using Alchemi.Core.Owner;
using ExpressionAnalisys;

namespace Alchemi.Grid.IntegralCalculator
{
    [Serializable]
    public class IntegralCalculationThread : GThread
    {
        private double start, end, result;
        private Expression expr;
        private const double step = 0.00001;

        public double Result
        {
            get { return result; }
        }

        public IntegralCalculationThread(double s, double e, Expression ex)
        {
            start = s;
            end = e;
            expr = ex;
            result = 0;
        }

        public override void Start()
        {
            var x = start;
            for (; x + step < end; x += step)
            {
                result += (expr.GetValue(x) + expr.GetValue(x + step)) * step / 2.0;
            }
            result += (expr.GetValue(x) + expr.GetValue(end)) * (end - x) / 2.0;
        }
    }

    class IntegralCalculation
    {
        static GApplication ga;
        static double sum = 0.0;

        [STAThread]
        static void Main(string[] args)
        {
            Console.WriteLine("Enter furmula: ");
            var formula = Console.ReadLine();
            var expr = new Expression(formula, new []{ "x"});

            Console.WriteLine("Enter interval [start end]");
            var border_input = Console.ReadLine().Split(' ');
            var start = Convert.ToDouble(border_input[0]);
            var end = Convert.ToDouble(border_input[1]);

            Console.WriteLine("Enter number of threads: ");
            var num_threads = Convert.ToInt32(Console.ReadLine());

            Console.WriteLine("[enter] to start grid application ...");
            Console.ReadLine();

            ga = new GApplication(new GConnection("localhost", 9000, "user", "user"));
            ga.ApplicationName = "Integral calculator";

            // add GridThread module (this executable) as a dependency
            ga.Manifest.Add(new ModuleDependency(typeof(IntegralCalculationThread).Module));

            var thread_step = (end - start) / num_threads;
            for (int i = 0; i < num_threads; i++)
            {
                var s = start + i * thread_step;
                var e = start + (i + 1) * thread_step;
                var thread = new IntegralCalculationThread(s, e, expr);

                ga.Threads.Add(thread);
            }

            ga.ThreadFinish += new GThreadFinish(ThreadFinished);
            ga.ThreadFailed += new GThreadFailed(ThreadFailed);
            ga.ApplicationFinish += new GApplicationFinish(ApplicationFinished);

            ga.Start();
            Console.ReadLine();
        }

        static void ThreadFinished(GThread th)
        {
            var thread = (IntegralCalculationThread)th;
            sum += thread.Result;
        }

        static void ThreadFailed(GThread th, Exception e)
        {
            Console.WriteLine("thread # {0} finished with error '{1}'", th.Id, e.Message);
        }

        static void ApplicationFinished()
        {
            Console.WriteLine("Result = {0}", sum.ToString("F5"));
            Console.WriteLine("\napplication finished");
            Console.WriteLine("\n[enter] to continue ...");
        }
    }
}
