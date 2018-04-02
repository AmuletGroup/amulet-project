package amulet.translator;

public class ErrorMsg
{
	String msg;
	int line;
	
	public ErrorMsg(String m, int l)
	{
		msg = m;
		line = l;
	}
	
	public String toString()
	{
		return "ERROR (line "+line+"): "+msg+".";
	}
}