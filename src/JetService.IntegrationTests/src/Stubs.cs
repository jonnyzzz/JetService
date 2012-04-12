namespace JetService.IntegrationTests
{
  public static class Stubs
  {
    public static string[] A(params string[] str)
    {
      return str;
    }

    public static void NOP<A>(A a)
    {
    }

    public static void NOP<A, B>(A a, B b)
    {
    }

    public static void NOP<A, B, C>(A a, B b, C c)
    {
    }

    public static void NOP<A, B, C, D>(A a, B b, C c, D d)
    {
    }
  }
}