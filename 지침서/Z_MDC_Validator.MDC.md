moduleContract:
  moduleName: "Z_MDC_Validator.MDC"
feature:
  featureName: "CoreMDCValidation"
  description: "Z0-Core 헌법에 따라 .MDC 파일의 유효성을 검증하는 CLI 도구"
# ...
interfaces:
  inputs:
    - kind: "DirectAPI"
      name: "Validator::run"
      params: ["targetFilePath", "schemaFilePath"]
  outputs:
    - kind: "EventBus" # CLI에서는 표준 출력(stdout/stderr)에 해당
      name: "ValidationResult::onComplete"
      payload: { struct: "ValidationPayload", fields: ["isValid", "errors"] }
# ...
tests:
  - id: "UT-VALIDATOR-PASS-01"
    type: "unit"
    scenario: "유효한 MDC 파일(UserAuthentication.MDC.md)을 검증하면 성공해야 한다."
  - id: "UT-VALIDATOR-FAIL-01"
    type: "unit"
    scenario: "requireIf 규칙을 위반한 헌법 파일을 검증하면 실패해야 한다."