using System;
using System.IO;

namespace JetService.IntegrationTests
{
  public static class TempFilesHolder
  {
    public static void WithTempFile(Action<string> action)
    {
      WithTempFile<Object>(y =>
                     {
                       action(y);
                       return null;
                     });
    }

    public static T WithTempFile<T>(Func<string, T> action)
    {
      string tmp = Path.GetTempFileName();
      try
      {
        return action(tmp);
      } finally
      {
        File.Delete(tmp);
      }
    }

    public static T WithTempDirectory<T>(Func<string, T> action)
    {
      string tmp = Path.GetTempFileName();
      File.Delete(tmp);
      Directory.CreateDirectory(tmp);
      try
      {
        return action(tmp);
      }
      finally
      {
        Directory.Delete(tmp, true);
      }
    }

    public static void WithTempDirectory(Action<string> action)
    {
      WithTempDirectory(t =>
                          {
                            action(t);
                            return t;
                          });
    }
  }
}