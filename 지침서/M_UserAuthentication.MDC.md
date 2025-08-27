

\[META INSTRUCTION BLOCK - FOR AI ONLY]

이 부록은 위 `Z0-Core` 헌법을 사용하여 실제 기능(`UserAuthentication`)을 어떻게 설계하는지 보여주는 \*\*검증 가능한(Verifiable) 참조 구현\*\*이다. 이 예시는 `validationMode: "strict"`의 모든 요구사항을 충족하며, 보강된 모든 스키마 규칙을 통과한다.



```yaml

\# ── 1. Common ───────────────────────────────────────────────

moduleContract:

&nbsp; moduleName: "AUTH"

feature:

&nbsp; featureName: "UserAuthentication"

&nbsp; description: "사용자 이메일과 비밀번호를 이용한 로그인 처리 및 세션 관리"

compatibility:

&nbsp; version: "AUTH.v1.0.0"

capabilities:

&nbsp; usesNetworking: true

&nbsp; usesHierarchicalTags: true

validationMode: "strict"



\# ── 2. Domain ───────────────────────────────────────────────

domain:

&nbsp; entities:

&nbsp;   - name: UserSession

&nbsp;     properties:

&nbsp;       - { name: UserID, type: string, tag: "Auth.Session.UserID" }

&nbsp;       - { name: AuthToken, type: string, tag: "Auth.Session.Token" }

&nbsp;       - { name: Status, type: enum, values: \["LoggedIn", "LoggedOut", "Expired"] }

&nbsp; rules:

&nbsp;   - id: "RULE-AUTH-01"

&nbsp;     description: "인증 토큰(AuthToken)은 발급 후 60분 뒤 만료된다."



\# ── 3. Architecture ───────────────────────────────────────────

interfaces:

&nbsp; inputs:

&nbsp;   - kind: "DirectAPI"

&nbsp;     name: "AuthService::RequestLogin"

&nbsp;     params: \["Email", "Password"]

&nbsp; outputs:

&nbsp;   - kind: "EventBus"

&nbsp;     name: "AuthEvents::OnLoginStatusChanged"

&nbsp;     payload:

&nbsp;       struct: "LoginStatusPayload"

&nbsp;       fields: \["UserID", "NewStatus", "AuthToken"]

pipeline:

&nbsp; - step: ValidateInput

&nbsp;   where: "MainThread"

&nbsp;   authority: "ClientOnly"

&nbsp;   effects:

&nbsp;     - "guard: if email or password format is invalid, throw ValidationException"

&nbsp; - step: AuthenticateWithService

&nbsp;   where: "BackgroundWorker"

&nbsp;   authority: "ClientOnly"

&nbsp;   effects:

&nbsp;     - "async: POST /api/auth/login with {Email, Password}"

&nbsp;     - "returns: {UserID, AuthToken}"

&nbsp; - step: UpdateLocalSession

&nbsp;   where: "MainThread"

&nbsp;   authority: "ClientOnly"

&nbsp;   effects:

&nbsp;     - "mutate: UserSession.Status = 'LoggedIn'"

&nbsp;     - "mutate: LocalStorage.AuthToken = AuthToken"

&nbsp;     - "emit: AuthEvents::OnLoginStatusChanged"

dataSchemas:

&nbsp; - id: "LoginStatusPayload"

&nbsp;   type: "struct"

&nbsp;   fields:

&nbsp;     - { name: "UserID", type: "string" }

&nbsp;     - { name: "NewStatus", type: "string" }

&nbsp;     - { name: "AuthToken", type: "string" }



\# ── 4. Quality ────────────────────────────────────────────────

performance:

&nbsp; metricsTable:

&nbsp;   - metric: "AuthenticateWithService.DurationMs"

&nbsp;     budget\_ms: 500

&nbsp;     timeout\_s: 10

tests:

&nbsp; - id: "IT-AUTH-LOGIN-01"

&nbsp;   type: "integration"

&nbsp;   scenario: "유효한 자격증명으로 로그인 시 세션이 활성화되고 이벤트가 발생한다."

&nbsp;   given: "User is LoggedOut"

&nbsp;   when: "AuthService::RequestLogin with valid credentials"

&nbsp;   then:

&nbsp;     - "UserSession.Status is 'LoggedIn'"

&nbsp;     - "AuthEvents::OnLoginStatusChanged is emitted with status 'LoggedIn'"



\# ── 5. Extensions ──────────────────────────────────────────────

extensions: {}

```

