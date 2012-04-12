using System;
using System.Threading;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class WaitFor
  {
    public static void WaitForAssert(Func<bool> predicate, string message, params object[] argz)
    {
      for(int i = 0; i < 6; i++)
      {
        if (predicate()) return;
        Thread.Sleep(TimeSpan.FromSeconds(.42 + i));
      }
      Assert.Fail(string.Format(message, argz));
    }

    public static void WaitForAssert(TimeSpan maxTime, TimeSpan interval, Func<bool> predicate, string message, params object[] argz)
    {
      for(TimeSpan sp = TimeSpan.Zero; sp < maxTime; sp += interval)
      {
        if (predicate()) return;
        Thread.Sleep(interval);                
      }
      Assert.Fail(string.Format(message, argz));
    }
  }
}