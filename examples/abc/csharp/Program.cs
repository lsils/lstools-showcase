using System;
using System.Json;
using System.Runtime.InteropServices;
using System.Text;

namespace abc
{
    public class ABC
    {
        private class Lib
        {
            [DllImport("../../../build/examples/abc/libabc2_c.dylib", EntryPoint = "abc2_create")]
            public static extern IntPtr abc2_create();

            [DllImport("../../../build/examples/abc/libabc2_c.dylib", EntryPoint = "abc2_delete")]
            public static extern void abc2_delete(IntPtr cli);

            [DllImport("../../../build/examples/abc/libabc2_c.dylib", EntryPoint = "abc2_command")]
            public static extern int abc2_command(IntPtr cli, string command, StringBuilder log, int size);
        }

        public ABC()
        {
            cli = Lib.abc2_create();
        }

        ~ABC()
        {
            Lib.abc2_delete(cli);
        }

        public JsonValue Run(string command)
        {
            StringBuilder read = new StringBuilder(4096);
            if ( Lib.abc2_command(cli, command, read, 4096) > 0 )
            {
                return JsonValue.Parse(read.ToString());
            }
            
            return null;
        }

        private IntPtr cli;
    }

    class Program
    {
        static void Main(string[] args)
        {
            var abc = new ABC();
            abc.Run("read_verilog ../function.v");
            abc.Run("ps -w");
            abc.Run("convert --wlc_to_aig");
            var nodes_before = abc.Run("ps -a")["nodes"];
            abc.Run("syn3");
            var nodes_after = abc.Run("ps -a")["nodes"];
            abc.Run("write_aiger /tmp/test.aig");

            if (nodes_after < nodes_before)
            {
                Console.WriteLine("Improvement :)");
            }
            else
            {
                Console.WriteLine("No improvement :(");
            }
        }
    }
}
