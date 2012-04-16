using System;
using System.Threading;
using NUnit.Framework;

namespace JetService.IntegrationTests
{
  public static class WaitFor
  {
    public static void WaitForAssert(Func<bool> predicate, Func<string> message)
    {
      var i = 0;
      Func<TimeSpan> interval = () => TimeSpan.FromSeconds(.42 + i++);
      WaitForAssert(TimeSpan.FromMinutes(3), interval, predicate, message);
    }
    
    public static void WaitForAssert(TimeSpan maxTime, Func<TimeSpan> interval, Func<bool> predicate, Func<string> message)
    {
      for(TimeSpan sp = TimeSpan.Zero; sp < maxTime; )
      {
        if (predicate()) return;
        TimeSpan sleep = interval();
        Thread.Sleep(sleep);
        sp += sleep;
      }
      Assert.Fail(message());
    }
  }
}