using System;
using System.Threading;

namespace JetService.IntegrationTests
{
  public static class ThreadUtil
  {
    public static T ExecuteSTA<T>(Func<T> action)
    {
      T result = default(T);
      Exception err = null;
      var th = new Thread(() =>
                            {
                              try
                              {
                                result = action();
                              } catch(Exception e)
                              {
                                err = e;
                              }
                            });
      th.IsBackground = false;
      th.SetApartmentState(ApartmentState.STA);
      th.Start();
      th.Join();
      if (err != null) throw new Exception("Failed to execute.", err);
      return result;
    }

    public static void ExecuteSTA(Action action)
    {
      ExecuteSTA(() =>
                   {
                     action();
                     return "";
                   });
    }
  }
}