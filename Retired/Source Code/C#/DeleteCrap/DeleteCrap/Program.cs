using System;
using System.IO;
using System.Linq;

namespace DeleteCrap
{
    class Program
    {
        static void Main()
        {
            string drive = FindCorrectDrive();

            if (!string.IsNullOrEmpty(drive))
                DeleteCrapFiles(drive);

            Console.WriteLine("ALL DONE");
            Console.ReadLine();
        }

        private static void DeleteCrapFiles(string drive)
        {
            var dir = new DirectoryInfo(drive);
            dir.GetFiles(".*", SearchOption.AllDirectories).ToList().ForEach(f => { Console.WriteLine("DELETING FILE: " + f.Name); f.Delete(); });
            dir.GetDirectories(".*", SearchOption.AllDirectories).ToList().ForEach(d => { Console.WriteLine("DELETING DIR : " + d.Name); d.Delete(true); });
        }

        private static string FindCorrectDrive()
        {
            string result = string.Empty;
            
            DriveInfo[] allDrives = DriveInfo.GetDrives();

            foreach (DriveInfo d in allDrives)
                if (d.IsReady && d.VolumeLabel == "R2D2 FILES")
                    result = d.Name;

            return result;
        }
    }
}
