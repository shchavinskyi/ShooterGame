using UnrealBuildTool;

public class ShooterFramework : ModuleRules
{
	public ShooterFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"Engine",
				"CoreUObject",
			}
		);
	}
}
