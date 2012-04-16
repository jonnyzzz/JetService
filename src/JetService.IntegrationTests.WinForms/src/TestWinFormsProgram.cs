using System;
using System.Windows.Forms;

namespace JetService.IntegrationTests.WinForms
{
  public enum WinFormsTestAction
  {
    TEST_OPEN
  }

  public static class TestWinFormsProgram
  {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main()
    {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      Application.Run(new Form1());
    }
  }
}
