using System;
using System.Collections.Generic;
using System.IO;

namespace SyncCommonFiles
{
    class Program
    {
        readonly static List<string> fileList = new List<string>();
        static string sourceFile;
        static DateTime sourceFileDate;

        static void Main()
        {
            LoadFileList();

            fileList.ForEach(file =>
            {
                SearchForSourceFile(file, Directory.GetCurrentDirectory());
                Console.WriteLine(String.Format("File: {0} Last Write: {1}", sourceFile, sourceFileDate));
                SearchForSourceFile(file, Directory.GetCurrentDirectory(), sourceFile);
            });

            Console.WriteLine("ALL DONE");
            Console.ReadLine();
        }

        private static void SearchForSourceFile(string fileNameToFind, string directoryToSearch, string source = "NONE")
        {
            Array.ForEach(Directory.GetDirectories(directoryToSearch), directory =>
            {
                Array.ForEach(Directory.GetFiles(directory, fileNameToFind), file =>
                {
                    if (source == "NONE")
                    {
                        DetermineIfLatestFile(file);
                    }
                    else
                    {
                        CopyFile(source, file);
                    }
                });
                SearchForSourceFile(fileNameToFind, directory, source);
            });
        }

        private static void DetermineIfLatestFile(string file)
        {
            var info = new FileInfo(file);
            if (string.IsNullOrEmpty(sourceFile) || sourceFileDate < info.LastWriteTime)
            {
                sourceFile = file;
                sourceFileDate = info.LastWriteTime;
            }
        }

        private static void CopyFile(string source, string file)
        {
            if (source == file)
                return;

            string path = file.Remove(0, Directory.GetCurrentDirectory().Length);
            Console.WriteLine("OVERWRITING: " + path);
            File.Copy(source, file, true);
        }

        private static void LoadFileList()
        {
            Console.WriteLine("Loading file list...");

            fileList.Add("I2C_Common.h");
        }
    }
}
