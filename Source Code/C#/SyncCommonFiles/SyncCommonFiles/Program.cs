using System;
using System.IO;

namespace SyncCommonFiles
{
    class Program
    {
        static string sourceFile;
        static DateTime sourceFileDate;

        static void Main()
        {
            SearchForSourceFile("I2C_Common.h", Directory.GetCurrentDirectory());

            Console.WriteLine(sourceFile);
            Console.WriteLine(sourceFileDate);

            SearchForSourceFile("I2C_Common.h", Directory.GetCurrentDirectory(), sourceFile);

            Console.WriteLine("ALL DONE");
            Console.ReadLine();
        }

        private static void SearchForSourceFile(string fileNameToFind, string directoryToSearch, string source = "NONE")
        {
            foreach (string d in Directory.GetDirectories(directoryToSearch))
            {
                foreach (string f in Directory.GetFiles(d, fileNameToFind))
                {
                    if (source == "NONE")
                    {
                        // get the date of the file
                        FileInfo info = new FileInfo(f);

                        if (string.IsNullOrEmpty(sourceFile) || sourceFileDate < info.LastWriteTime)
                        {
                            sourceFile = f;
                            sourceFileDate = info.LastWriteTime;
                        }
                    }
                    else if (source != f)
                    {
                        string path = f.Remove(0, Directory.GetCurrentDirectory().Length);

                        Console.WriteLine("OVERWRITING: " + path);
                        File.Copy(source, f, true);
                    }
                }

                SearchForSourceFile(fileNameToFind, d, source);
            }
        }
    }
}
