const RuleCondition = {
    Less: 0,
    LessEqual: 1,
    Equal: 2,
    GreaterEqual: 3,
    Greater: 4,
    OneFromList: 5,
    AllFromList: 6,
    TotalConditions: 7
};

const RuleType = {
    Allow: 0,
    Deny: 1,
    TotalTypes: 2
};

const LicenseState = {
    Reserved: 0,
    Active: 1,
    Revoked: 2,
    TotalStates: 3
};

const UUIDS = {
    NotValidUUID: 0
}
module.exports = {
    RuleCondition,
    RuleType,
    LicenseState,
    UUIDS
};